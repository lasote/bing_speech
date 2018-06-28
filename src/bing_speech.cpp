#include <iostream>
#include <vector>
#include <curl/curl.h>

#include "../include/bing_speech.h"
#include "rapidjson/document.h"


using namespace rapidjson;

BingSpeech::BingSpeech(std::string the_lang, std::string the_api_key, bool the_verbose_curl){
    lang = the_lang;
    api_key = the_api_key;
    verbose_curl = the_verbose_curl;
}

size_t callback_str(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

std::string BingSpeech::get_text(std::vector<int16_t> data){

    //curl -v -X POST "https://speech.platform.bing.com/speech/recognition/interactive/cognitiveservices/v1?language=en-us&format=detailed"
    //-H "Transfer-Encoding: chunked" -H "Ocp-Apim-Subscription-Key: YOUR_SUBSCRIPTION_KEY" -H "Content-type: audio/wav; codec=audio/pcm; samplerate=16000" --data-binary @YOUR_AUDIO_FILE

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(!curl) {
        std::cerr << "Unable to open playback device!" << std::endl;

    }
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    std::string url = "https://speech.platform.bing.com/speech/"
                              "recognition/interactive/cognitiveservices/v1?language=" + lang + "&format=simple";

    headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
    headers = curl_slist_append(headers, ("Ocp-Apim-Subscription-Key: " + api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: audio/wav; codec=audio/pcm; samplerate=16000");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size()*2);
    int16_t* data_p = &data[0];
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_p);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string s;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_str);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    if(verbose_curl)
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* Perform the request, res will get the return code */
    std::cout << "Sending data!" << std::endl;;
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    std::cout << res << std::endl;

    /* Check for errors */
    if(res != CURLE_OK){
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        return "";
    }
    else{
        std::cout << "Retorno=>" << s.c_str() << std::endl;
        Document d;
        d.Parse(s.c_str());
        if(d["RecognitionStatus"] != "Success"){
            std::cout << "Error!: " << std::endl << s << std::endl;
            return "";
        }

        Value& w = d["DisplayText"];
        return w.GetString();
    }

}
