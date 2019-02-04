#include "config.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/locale/generator.hpp>

#include <cstdlib>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>

using namespace std;
using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;

const string host{HOST_DEFAULT};

const array<string, 105> langs{
    "auto", //Automatic
    "af", //Afrikaans
    "sq", //Albanian
    "am", //Amharic
    "ar", //Arabic
    "hy", //Armenian
    "az", //Azerbaijani
    "eu", //Basque
    "be", //Belarusian
    "bn", //Bengali
    "bs", //Bosnian
    "bg", //Bulgarian
    "ca", //Catalan
    "ceb", //Cebuano
    "ny", //Chichewa
    "zh-cn", //Chinese Simplified
    "zh-tw", //Chinese Traditional
    "co", //Corsican
    "hr", //Croatian
    "cs", //Czech
    "da", //Danish
    "nl", //Dutch
    "en", //English
    "eo", //Esperanto
    "et", //Estonian
    "tl", //Filipino
    "fi", //Finnish
    "fr", //French
    "fy", //Frisian
    "gl", //Galician
    "ka", //Georgian
    "de", //German
    "el", //Greek
    "gu", //Gujarati
    "ht", //Haitian Creole
    "ha", //Hausa
    "haw", //Hawaiian
    "iw", //Hebrew
    "hi", //Hindi
    "hmn", //Hmong
    "hu", //Hungarian
    "is", //Icelandic
    "ig", //Igbo
    "id", //Indonesian
    "ga", //Irish
    "it", //Italian
    "ja", //Japanese
    "jw", //Javanese
    "kn", //Kannada
    "kk", //Kazakh
    "km", //Khmer
    "ko", //Korean
    "ku", //Kurdish (Kurmanji)
    "ky", //Kyrgyz
    "lo", //Lao
    "la", //Latin
    "lv", //Latvian
    "lt", //Lithuanian
    "lb", //Luxembourgish
    "mk", //Macedonian
    "mg", //Malagasy
    "ms", //Malay
    "ml", //Malayalam
    "mt", //Maltese
    "mi", //Maori
    "mr", //Marathi
    "mn", //Mongolian
    "my", //Myanmar (Burmese)
    "ne", //Nepali
    "no", //Norwegian
    "ps", //Pashto
    "fa", //Persian
    "pl", //Polish
    "pt", //Portuguese
    "ma", //Punjabi
    "ro", //Romanian
    "ru", //Russian
    "sm", //Samoan
    "gd", //Scots Gaelic
    "sr", //Serbian
    "st", //Sesotho
    "sn", //Shona
    "sd", //Sindhi
    "si", //Sinhala
    "sk", //Slovak
    "sl", //Slovenian
    "so", //Somali
    "es", //Spanish
    "su", //Sundanese
    "sw", //Swahili
    "sv", //Swedish
    "tg", //Tajik
    "ta", //Tamil
    "te", //Telugu
    "th", //Thai
    "tr", //Turkish
    "uk", //Ukrainian
    "ur", //Urdu
    "uz", //Uzbek
    "vi", //Vietnamese
    "cy", //Welsh
    "xh", //Xhosa
    "yi", //Yiddish
    "yo", //Yoruba
    "zu" //Zulu
};

string from, to;
unsigned t0, t1;

bool valid_language(const string & lang) {
    return find(langs.begin(), langs.end(), lang);
}

inline char digit(int n) {
    return n > 9 ? 'A' - 10 + n : '0' + n;
}

string build_url(const string & token, const string & text) {
    ostringstream os;
    os << "/translate_a/single?client=t&sl=" << from << "&tl=" << to << "&hl=" << to
        << "&dt=at&dt=bd&dt=ex&dt=ld&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&otf=1&ssel=0&tsel=0&kc=7&tk="
        << token << "&q=";
    for (unsigned char c : text) {
        os.put('%');
        os.put(digit(c/16));
        os.put(digit(c%16));
    }
    return os.str();
}

string cal_token(const string & text) {
    unsigned a{t0};
    for (unsigned char c : text) {
        a += c; a += a << 10; a ^= a >> 6;
    }
    a += a << 3; a ^= a >> 11; a += a << 15; a ^= t1; a %= 1000000;
    return to_string(a) + "." + to_string(a ^ t0);
}

void get_tkk(ssl::stream<tcp::socket> & stream) {
    http::request<http::string_body> req{http::verb::get, "/", 11};
    req.set(http::field::host, host);
    
    http::write(stream, req);
    
    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);
    
    regex rgx{"TKK[:=]'(\\d+?).(\\d+?)'", regex::icase | regex::optimize};
    smatch sm;
    regex_search(res.body(), sm, rgx);

    t0 = stoul(sm.str(1)), t1 = stoul(sm.str(2));
}

string translate(ssl::stream<tcp::socket> & stream, const string & text) {
    string token = cal_token(text);
    string url = build_url(token, text);
    
    http::request<http::string_body> req{http::verb::get, url, 11};
    req.set(http::field::host, host);
    
    http::write(stream, req);
    
    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);
    
    boost::property_tree::ptree tree;
    istringstream is{res.body()};
    boost::property_tree::read_json(is, tree); 
    
    return tree.front().second.front().second.front().second.get_value<string>();
}

int main(int argc, char ** argv) {
    boost::locale::generator gen;
    locale loc = gen("");
    cin.imbue(loc);
    cout.imbue(loc);
    
    string text;
    if (argc == 1) from = LANGUAGE_DEFAULT_FROM, to = LANGUAGE_DEFAULT_TO;
    else if (argc == 3) from = argv[1], to = argv[2];
    else {
        cerr << "Usage: translate [<from>] [<to>]" << endl;
        return EXIT_FAILURE;
    }
    if (!valid_language(from) || !valid_language(to)) {
        cerr << "Error: Unsupported languages! " << endl;
        return EXIT_FAILURE;
    }
    try {
        boost::asio::io_context ioc;
        //SSL
        ssl::context ctx{ssl::context::sslv23_client};
        ctx.set_default_verify_paths();
        ctx.set_verify_mode(ssl::verify_none);
        ssl::stream<tcp::socket> stream{ioc, ctx};
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }
        //DNS
        tcp::resolver resolver{ioc};
        auto const results = resolver.resolve(host, "443");
        boost::asio::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(ssl::stream_base::client);
        //CHECK
        get_tkk(stream);
        //TRANSLATE
        while (getline(cin, text) && !all_of(begin(text), end(text), [](char c){ return c == ' '; }))
            cout << translate(stream, text) << endl;
        //CLOSE
        boost::system::error_code ec;
        stream.shutdown(ec);
        if (ec == boost::asio::error::eof) ec.assign(0, ec.category());
        if (ec && ec != ssl::error::stream_truncated) throw boost::system::system_error{ec};
    } catch (exception const & e) {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
