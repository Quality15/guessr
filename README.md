# Guessr

## Build (only on Linux)

```
$ git clone https://github.com/Quality15/guessr
$ cd guessr
$ make
```

## Host a server

You'll be a server and another 2 players will connect to you

Port - `4444` (may change in `main.cpp`)

```
$ ./guessr server
```

## Join as player

```
$ ./guessr client
Enter host ip: *ip of your host server*
*Game will tell who you are (guesser or chooser)*
```