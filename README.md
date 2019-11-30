# HTTP Server
still wip

## GET

```
GET index.html HTTP/1.1
Host: www.nowhere123.com
Accept: image/gif, image/jpeg
Accept-Language: en-us
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)
```

```
HTTP/1.1 200 OK
Last-Modified: Sat, 20 Nov 2019 07:16:26 GMT
ETag: "10000000565a5-2c-3e94b66c2e680"
Accept-Ranges: bytes
Content-Length: 44
Connection: close
Content-Type: text/html
<html><body><h1>It works!</h1></body></html>
```
