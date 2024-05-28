# puzzle box client

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

