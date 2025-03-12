#include <stdio.h>
#include "http_parser.h"
#include <string>
#include <map>


// ���ڽ�����ȫ�ֱ���
std::map<std::string, std::string> mapReqHeadField;
std::string strReqUrl;
std::string strReqBody;
std::string strReqFieldKey;

std::map<std::string, std::string> mapRespHeadField;
std::string strRespStatus;
std::string strRespBody;
std::string strRespFieldKey;

// ���ڽ���http����Ļص�����
int onReqMessageBegin(http_parser* pParser);
int onReqHeaderComplete(http_parser* pParser);
int onReqMessageComplete(http_parser* pParser);
int onReqURL(http_parser* pParser, const char* at, size_t length);
int onReqHeaderField(http_parser* pParser, const char* at, size_t length);
int onReqHeaderValue(http_parser* pParser, const char* at, size_t length);
int onReqBody(http_parser* pParser, const char* at, size_t length);

// ���ڽ���http��Ӧ�Ļص�����
int onRespMessageBegin(http_parser* pParser);
int onRespHeaderComplete(http_parser* pParser);
int onRespMessageComplete(http_parser* pParser);
int onRespStatus(http_parser* pParser, const char* at, size_t length);
int onRespHeaderField(http_parser* pParser, const char* at, size_t length);
int onRespHeaderValue(http_parser* pParser, const char* at, size_t length);
int onRespBody(http_parser* pParser, const char* at, size_t length);


int main(int argc, char* argv[])
{
    // ����http����

    // ��������������
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

    // ��ʹ��������
    http_parser_init(&httpReqParser, HTTP_REQUEST);
    // ���ûص�����
    http_parser_settings_init(&httpReqSettings);
    httpReqSettings.on_message_begin = onReqMessageBegin;
    httpReqSettings.on_headers_complete = onReqHeaderComplete;
    httpReqSettings.on_message_complete = onReqMessageComplete;
    httpReqSettings.on_url = onReqURL;
    httpReqSettings.on_header_field = onReqHeaderField;
    httpReqSettings.on_header_value = onReqHeaderValue;
    httpReqSettings.on_body = onReqBody;

    // ��������
    size_t reqSize = strHttpReq.size();
    size_t nParseSize = http_parser_execute(&httpReqParser, &httpReqSettings, strHttpReq.c_str(), reqSize);
    if (nParseSize < reqSize) {
        printf("http_parser_execute http request failed.\n");
        return -1;
    }

    printf("origin message:\n%s\n", strHttpReq.c_str());
    printf("==========================================\n");


    // �����ɹ�����ӡ�������
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

    // ����http��Ӧ


    // ����������Ӧ����
    std::string strHttpResponse;
    strHttpResponse += "HTTP/1.1 200 OK\r\n";
    strHttpResponse += "Server: nginx/1.18.0\r\n";
    strHttpResponse += "Accept: */*\r\n";
    strHttpResponse += "Connection: keep-alive\r\n";
    strHttpResponse += "\r\n";
    strHttpResponse += "{\"respmsg\": \"Welcome to http-parser\"}\r\n";

    http_parser httpRespParser;
    http_parser_settings httpRespSettings;

    // ��ʹ��������
    http_parser_init(&httpRespParser, HTTP_RESPONSE);
    // ���ûص�����
    http_parser_settings_init(&httpRespSettings);
    httpRespSettings.on_message_begin = onRespMessageBegin;
    httpRespSettings.on_headers_complete = onRespHeaderComplete;
    httpRespSettings.on_message_complete = onRespMessageComplete;
    httpRespSettings.on_status = onRespStatus;
    httpRespSettings.on_header_field = onRespHeaderField;
    httpRespSettings.on_header_value = onRespHeaderValue;
    httpRespSettings.on_body = onRespBody;

    // ������Ӧ
    int responseSize = strHttpResponse.size();
    nParseSize = http_parser_execute(&httpRespParser, &httpRespSettings, strHttpResponse.c_str(), responseSize);
    if (nParseSize < responseSize) {
        printf("http_parser_execute http response failed.\n");
        return -1;
    }

    // �����ɹ�����ӡ�������
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
    // ��ʼ��������
    printf("onReqMessageBegin call \n");
    return 0;
}
int onReqHeaderComplete(http_parser* pParser)
{
    // ����ͷ�������
    // HTTP����ͷ�������� \r\n ��β�� ��������������� \r\n, ˵��http���ĸ�ʽ��������߱��Ĳ�����������ص����ᱻ����
    printf("onReqHeaderComplete call \n");
    return 0;
}
int onReqMessageComplete(http_parser* pParser)
{
    // ȫ���������
    printf("onReqMessageComplete call \n");
    return 0;
}
int onReqURL(http_parser* pParser, const char* at, size_t length)
{
    // ����URL
    strReqUrl.assign(at, length);
    return 0;
}

int onReqHeaderField(http_parser* pParser, const char* at, size_t length)
{
    // ��������ͷkey
    strReqFieldKey.assign(at, length);
    return 0;
}
int onReqHeaderValue(http_parser* pParser, const char* at, size_t length)
{
    // ��������ͷvalue
    std::string strValue(at, length);
    mapReqHeadField.insert(std::make_pair(strReqFieldKey, strValue));
    return 0;
}
int onReqBody(http_parser* pParser, const char* at, size_t length)
{
    // �����������Ӧ��
    strReqBody.append(at, length);
    return 0;
}


// ==================

int onRespMessageBegin(http_parser* pParser)
{
    // ��ʼ��������
    printf("onRespMessageBegin call \n");
    return 0;
}
int onRespHeaderComplete(http_parser* pParser)
{
    // ����ͷ�������
    // HTTP����ͷ�������� \r\n ��β�� ��������������� \r\n, ˵��http���ĸ�ʽ��������߱��Ĳ�����������ص����ᱻ����
    printf("onRespHeaderComplete call \n");
    return 0;
}
int onRespMessageComplete(http_parser* pParser)
{
    // ȫ���������
    printf("onRespMessageComplete call \n");
    return 0;
}

int onRespStatus(http_parser* pParser, const char* at, size_t length)
{
    // ������Ӧ״̬��
    strRespStatus.assign(at, length);
    return 0;
}
int onRespHeaderField(http_parser* pParser, const char* at, size_t length)
{
    // ������Ӧͷkey
    strRespFieldKey.assign(at, length);
    return 0;
}
int onRespHeaderValue(http_parser* pParser, const char* at, size_t length)
{
    // ������Ӧͷvalue
    std::string strValue(at, length);
    mapRespHeadField.insert(std::make_pair(strRespFieldKey, strValue));
    return 0;
}
int onRespBody(http_parser* pParser, const char* at, size_t length)
{
    // �����������Ӧ��
    strRespBody.append(at, length);
    return 0;
}
