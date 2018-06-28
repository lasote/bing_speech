#include <string>


class BingSpeech {
    public:
        BingSpeech(std::string lang, std::string api_key, bool verbose_curl=false);
        std::string get_text(std::vector<int16_t> data);

    private:
        std::string api_key;
        std::string lang;
        bool verbose_curl;
};

size_t callback_str(void *contents, size_t size, size_t nmemb, std::string *s);

