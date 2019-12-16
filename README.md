# HTTP Server
Opens a TCP connection on `127.0.0.1:80`.

Serves files in the `www` directory.

```
.
├── LICENSE
├── Makefile
├── README.md
├── dist
│   └── server
├── src
│   ├── parser.c
│   └── server.c
└── www
    └── index.html

3 directories, 7 files
```

## Install
```
$ make
```

## Usage
```
$ ./dist/server

Starting server...
Listening on port 80...

```

## GET

## Request

```
GET index.html HTTP/1.1
Host: 127.0.0.1
Accept: image/gif, image/jpeg
Accept-Language: en-us
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)
```

## Response

```
HTTP/1.1 200 OK
Last-Modified: Sat, 20 Nov 2019 07:16:26 GMT
ETag: "10000000565a5-2c-3e94b66c2e680"
Accept-Ranges: bytes
Content-Length: 44
Connection: close
Content-Type: text/html
[html document]
```

## License 

  * This project is licensed under a BSD 3-Clause License

Copyright (c) 2019, John Musgrave All rights reserved.
 
