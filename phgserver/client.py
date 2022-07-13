import http.client
def HTTPPOST(ip):
    headers = {
        "Connection": "keep-alive",
        "Content-Type": "text/plain"
    }
    conn = http.client.HTTPConnection(ip + ':8080')
    phg = "{}>hello world;"
    conn.request('POST', '/cmd', phg, headers)
    res = conn.getresponse()
    content = res.read()
    print(content)
    conn.close()

if __name__ == '__main__':
    HTTPPOST('127.0.0.1')