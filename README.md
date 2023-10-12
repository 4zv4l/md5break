# md5break

md5 client/server breaker (university project)

## why

to do some `C` and `Ruby`, did the prototype in `Ruby` then made it in `C`.

## how

- server: use a file containing `md5 hashes` that it will send to clients
- client: use a file containing `passwords` to break the `md5 hash`

### example

- C  
`./client/client -f wordlist 127.0.0.1 8080`  
`./server/server -f hashes 127.0.0.1 8080`  

- Ruby  
`./md5_client/md5client.rb 127.0.0.1 8080 md5_client/wordlist`  
`./md5_server/md5server.rb 127.0.0.1 8080 md5_server/hashes`  

## todo

- [X] Ruby prototype
- [X] C 'prototype'
- [ ] Add `Async`/`Threads` for multi-client support
- [ ] Add support for other type of hashes
- [ ] Check for `bugs`
