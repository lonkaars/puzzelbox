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

## Send data

```
     ADDRESS  DATA
     v~~~     v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
send 0x39     68:65:6c:6c:6f  44      0x20    'world'    33
              ^~~~~~~~~~~~~~  ^~      ^~~~    ^~~~~~~    ^~
              HEXSTR          NUMBER  NUMBER  STRING     NUMBER
              (binary)        (dec)   (hex)   (literal)  (dec)
```

The data is concatenated, and may contain mixed types of literals

