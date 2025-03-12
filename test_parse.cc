#include <stdio.h>
#include "http_parser.h"
#include <string>
#include <map>


// 用于解析的全局变量
std::map<std::string, std::string> mapReqHeadField;
std::string strReqUrl;
std::string strReqBody;
std::string strReqFieldKey;

std::map<std::string, std::string> mapRespHeadField;
std::string strRespStatus;
std::string strRespBody;
std::string strRespFieldKey;

// 用于解析http请求的回调函数
int onReqMessageBegin(http_parser* pParser);
int onReqHeaderComplete(http_parser* pParser);
int onReqMessageComplete(http_parser* pParser);
int onReqURL(http_parser* pParser, const char* at, size_t length);
int onReqHeaderField(http_parser* pParser, const char* at, size_t length);
int onReqHeaderValue(http_parser* pParser, const char* at, size_t length);
int onReqBody(http_parser* pParser, const char* at, size_t length);

// 用于解析http响应的回调函数
int onRespMessageBegin(http_parser* pParser);
int onRespHeaderComplete(http_parser* pParser);
int onRespMessageComplete(http_parser* pParser);
int onRespStatus(http_parser* pParser, const char* at, size_t length);
int onRespHeaderField(http_parser* pParser, const char* at, size_t length);
int onRespHeaderValue(http_parser* pParser, const char* at, size_t length);
int onRespBody(http_parser* pParser, const char* at, size_t length);


int main(int argc, char* argv[])
{
    // 解析http请求

    // 待解析的请求报文
    std::string strHttpReq;
    strHttpReq += "POST /http-parser HTTP/1.1\r\n";
    strHttpReq += "Host: 127.0.0.1:10010\r\n";
    strHttpReq += "Accept: */*\r\n";
    strHttpReq += "Content-Type: application/json\r\n";
    strHttpReq += "Content-Length: 25\r\n";
    strHttpReq += "\r\n";
    strHttpReq += "{\"reqmsg\": \"Hello World\"}";

    http_parser httpReqParser;
    http_parser_settings httpReqSettings;

    // 初使化解析器
    http_parser_init(&httpReqParser, HTTP_REQUEST);
    // 设置回调函数
    http_parser_settings_init(&httpReqSettings);
    httpReqSettings.on_message_begin = onReqMessageBegin;
    httpReqSettings.on_headers_complete = onReqHeaderComplete;
    httpReqSettings.on_message_complete = onReqMessageComplete;
    httpReqSettings.on_url = onReqURL;
    httpReqSettings.on_header_field = onReqHeaderField;
    httpReqSettings.on_header_value = onReqHeaderValue;
    httpReqSettings.on_body = onReqBody;

    // 解析请求
    size_t reqSize = strHttpReq.size();
    size_t nParseSize = http_parser_execute(&httpReqParser, &httpReqSettings, strHttpReq.c_str(), reqSize);
    if (nParseSize < reqSize) {
        printf("http_parser_execute http request failed.\n");
        return -1;
    }

    printf("origin message:\n%s\n", strHttpReq.c_str());
    printf("==========================================\n");


    // 解析成功，打印解析结果
    if (httpReqParser.method == HTTP_GET) {
        printf("method: Get\n");
    }
    else if (httpReqParser.method == HTTP_POST) {
        printf("method: Post\n");
    }
    else if (httpReqParser.method == HTTP_HEAD) {
        printf("method: Head\n");
    }
    else {
        printf("method: other\n");
    }

    printf("url: %s \n", strReqUrl.c_str());
    printf("req heads:\n");
    for (std::map<std::string, std::string>::iterator iter = mapReqHeadField.begin(); iter != mapReqHeadField.end(); ++iter) {
        printf("\t %s : %s \n", iter->first.c_str(), iter->second.c_str());
    }
    printf("req body: %s \n", strReqBody.c_str());

    printf("==========================================\n");

    // 解析http响应


    // 待解析的响应报文
    std::string strHttpResponse;
    strHttpResponse += "HTTP/1.1 200 OK\r\n";
    strHttpResponse += "Server: nginx/1.18.0\r\n";
    strHttpResponse += "Accept: */*\r\n";
    strHttpResponse += "Connection: keep-alive\r\n";
    strHttpResponse += "\r\n";
    strHttpResponse += "{\"respmsg\": \"Welcome to http-parser\"}\r\n";

    http_parser httpRespParser;
    http_parser_settings httpRespSettings;

    // 初使化解析器
    http_parser_init(&httpRespParser, HTTP_RESPONSE);
    // 设置回调函数
    http_parser_settings_init(&httpRespSettings);
    httpRespSettings.on_message_begin = onRespMessageBegin;
    httpRespSettings.on_headers_complete = onRespHeaderComplete;
    httpRespSettings.on_message_complete = onRespMessageComplete;
    httpRespSettings.on_status = onRespStatus;
    httpRespSettings.on_header_field = onRespHeaderField;
    httpRespSettings.on_header_value = onRespHeaderValue;
    httpRespSettings.on_body = onRespBody;

    // 解析响应
    int responseSize = strHttpResponse.size();
    nParseSize = http_parser_execute(&httpRespParser, &httpRespSettings, strHttpResponse.c_str(), responseSize);
    if (nParseSize < responseSize) {
        printf("http_parser_execute http response failed.\n");
        return -1;
    }

    // 解析成功，打印解析结果
    printf("code: %d\n", httpRespParser.status_code);
    printf("status: %s \n", strRespStatus.c_str());
    printf("resp heads:\n");
    for (std::map<std::string, std::string>::iterator iter = mapRespHeadField.begin(); iter != mapRespHeadField.end(); ++iter) {
        printf("\t %s : %s \n", iter->first.c_str(), iter->second.c_str());
    }
    printf("resp body: %s \n", strRespBody.c_str());

    return 0;
}


int onReqMessageBegin(http_parser* pParser)
{
    // 开始解析报文
    printf("onReqMessageBegin call \n");
    return 0;
}
int onReqHeaderComplete(http_parser* pParser)
{
    // 报文头解析完成
    // HTTP报文头是以两个 \r\n 结尾， 如果解析不到两个 \r\n, 说明http报文格式有问题或者报文不完整，这个回调不会被调用
    printf("onReqHeaderComplete call \n");
    return 0;
}
int onReqMessageComplete(http_parser* pParser)
{
    // 全部解析完成
    printf("onReqMessageComplete call \n");
    return 0;
}
int onReqURL(http_parser* pParser, const char* at, size_t length)
{
    // 解析URL
    strReqUrl.assign(at, length);
    return 0;
}

int onReqHeaderField(http_parser* pParser, const char* at, size_t length)
{
    // 解析请求头key
    strReqFieldKey.assign(at, length);
    return 0;
}
int onReqHeaderValue(http_parser* pParser, const char* at, size_t length)
{
    // 解析请求头value
    std::string strValue(at, length);
    mapReqHeadField.insert(std::make_pair(strReqFieldKey, strValue));
    return 0;
}
int onReqBody(http_parser* pParser, const char* at, size_t length)
{
    // 解析请求或响应体
    strReqBody.append(at, length);
    return 0;
}


// ==================

int onRespMessageBegin(http_parser* pParser)
{
    // 开始解析报文
    printf("onRespMessageBegin call \n");
    return 0;
}
int onRespHeaderComplete(http_parser* pParser)
{
    // 报文头解析完成
    // HTTP报文头是以两个 \r\n 结尾， 如果解析不到两个 \r\n, 说明http报文格式有问题或者报文不完整，这个回调不会被调用
    printf("onRespHeaderComplete call \n");
    return 0;
}
int onRespMessageComplete(http_parser* pParser)
{
    // 全部解析完成
    printf("onRespMessageComplete call \n");
    return 0;
}

int onRespStatus(http_parser* pParser, const char* at, size_t length)
{
    // 解析响应状态码
    strRespStatus.assign(at, length);
    return 0;
}
int onRespHeaderField(http_parser* pParser, const char* at, size_t length)
{
    // 解析响应头key
    strRespFieldKey.assign(at, length);
    return 0;
}
int onRespHeaderValue(http_parser* pParser, const char* at, size_t length)
{
    // 解析响应头value
    std::string strValue(at, length);
    mapRespHeadField.insert(std::make_pair(strRespFieldKey, strValue));
    return 0;
}
int onRespBody(http_parser* pParser, const char* at, size_t length)
{
    // 解析请求或响应体
    strRespBody.append(at, length);
    return 0;
}
