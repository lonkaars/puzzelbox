# puzzle box client

goal (in order of implementation):
```
(pbc) help
    exit                exit pbc
    test                send a test puzbus message
    help                show this help
    send <addr> <data>  [debug] send raw message
    status              show global puzzle box state (main controller state)
    reset               reset entire game state
    ls                  list connected puzzle modules
```


```
send 0x39 "Hello world!" de:ad:be:ef 0xff     5        0a       0750
          ^~~~~~~~~~~~~~ ^~~~~~~~~~~ ~^~~     ~^       ~^       ~~~~^
          STR_INTP       BYTE_ARR    UNSIGNED UNSIGNED UNSIGNED UNSIGNED
                                     (hex+0x) (dec)    (hex)    (oct)
```
