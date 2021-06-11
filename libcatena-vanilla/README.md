# LibCatena

This is a simple toy blockchain implementation in C++ for learning purposes. I used the following as starting points:

* https://davenash.com/2017/10/build-a-blockchain-with-c/
* https://github.com/tko22/simple-blockchain

This is (quite literally) a weekend hack. Have done basic unit testing (shamefully included in the main.cpp itself :P).

The goal was to understand what blockchain technology is and how the basics work.

## Author
Saurav Mohapatra (@mohaps)

## Requirements
C++1y
Mac OS/Linux (Should compile in windows, haven't tried)

## License
APACHE 2.0 - http://www.apache.org/licenses/LICENSE-2.0

## How to build

```
$ make all
```
will build the bin/catena executable that can then be run as
```
$ bin/catena
```

```
$ make rebuild && bin/catena
```

is a handy command to rebuild and run the executable

## What does it do?
It will try to instantiate a block chain with a genesis block and then loop till it mines and adds 9 more blocks to the block chain.

Sample output (on a Core i5 DELL Inspiron Desktop with 8GB ram):
```[PROGRESS] [Worker #0] => (loops=7000000) (nonce=27999996) (elapsed = 27.5939 seconds) ... 
[PROGRESS] [Worker #2] => (loops=7000000) (nonce=27999998) (elapsed = 27.7868 seconds) ... 
[PROGRESS] [Worker #1] => (loops=7000000) (nonce=27999997) (elapsed = 27.8374 seconds) ... 
[PROGRESS] [Worker #3] => (loops=7000000) (nonce=27999999) (elapsed = 28.8489 seconds) ... 

BlockChain : [Catena Test] (difficulty=6)
Block Count => 1

  Block #0 {
    previous_hash => 0000000000000000000000000000000000000000000000000000000000000000
    current_hash  => 000000a2da8a8707c0b5101842b0a67cd34d7c6de7339632f64e70f826a69a4f
    nonce => 36039369
    data (2 entries) => [
      "Genesis_Block",
      "Catena Test",
    ]
  }

... Mining for block #1...
*** Block #1 added to blockchain!

  Block #1 {
    previous_hash => 000000a2da8a8707c0b5101842b0a67cd34d7c6de7339632f64e70f826a69a4f
    current_hash  => 00000020673a833ef674e17142f5c8b49f1ccc999a17476dbad93f109499e9f2
    nonce => 26738181
    data (1 entries) => [
      "Block #1",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #1( in 26.9311 s )
... Mining for block #2...
*** Block #2 added to blockchain!

  Block #2 {
    previous_hash => 00000020673a833ef674e17142f5c8b49f1ccc999a17476dbad93f109499e9f2
    current_hash  => 0000003344bbd1c977a0be7b92415f313d69f62a2ded3add9e27e9c19be00a3a
    nonce => 17959210
    data (1 entries) => [
      "Block #2",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #2( in 17.8624 s )
... Mining for block #3...
*** Block #3 added to blockchain!

  Block #3 {
    previous_hash => 0000003344bbd1c977a0be7b92415f313d69f62a2ded3add9e27e9c19be00a3a
    current_hash  => 0000008fa567de30f20f265263771e5c3d9d8ab1999009c56d16b493fad4237f
    nonce => 13415392
    data (1 entries) => [
      "Block #3",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #3( in 13.6919 s )
... Mining for block #4...
*** Block #4 added to blockchain!

  Block #4 {
    previous_hash => 0000008fa567de30f20f265263771e5c3d9d8ab1999009c56d16b493fad4237f
    current_hash  => 0000004d52c0071edc279dcd4caa16c99e0bf81dbb376a7e0435dc4d1453fb48
    nonce => 5662802
    data (1 entries) => [
      "Block #4",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #4( in 5.50717 s )
... Mining for block #5...
*** Block #5 added to blockchain!

  Block #5 {
    previous_hash => 0000004d52c0071edc279dcd4caa16c99e0bf81dbb376a7e0435dc4d1453fb48
    current_hash  => 000000ec9e4ed64ae764363ac3ea7e0375b1269424617d51282fcf5699eeb7f0
    nonce => 21183430
    data (1 entries) => [
      "Block #5",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #5( in 21.9852 s )
... Mining for block #6...
*** Block #6 added to blockchain!

  Block #6 {
    previous_hash => 000000ec9e4ed64ae764363ac3ea7e0375b1269424617d51282fcf5699eeb7f0
    current_hash  => 000000ffb1ce3cb5330df2422496eb79a08317be1d00a530de17ba8d2aa54900
    nonce => 16679565
    data (1 entries) => [
      "Block #6",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #6( in 16.4543 s )
... Mining for block #7...
*** Block #7 added to blockchain!

  Block #7 {
    previous_hash => 000000ffb1ce3cb5330df2422496eb79a08317be1d00a530de17ba8d2aa54900
    current_hash  => 00000017adea336ebc16be447b82e985cd9af0ab4a3def6ca40bedb294d2a3e1
    nonce => 3741020
    data (1 entries) => [
      "Block #7",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #7( in 3.60823 s )
... Mining for block #8...
[PROGRESS] [Worker #1] => (loops=7000000) (nonce=27999997) (elapsed = 26.921 seconds) ... 
[PROGRESS] [Worker #3] => (loops=7000000) (nonce=27999999) (elapsed = 27.2616 seconds) ... 
[PROGRESS] [Worker #0] => (loops=7000000) (nonce=27999996) (elapsed = 27.5134 seconds) ... 
[PROGRESS] [Worker #2] => (loops=7000000) (nonce=27999998) (elapsed = 28.0264 seconds) ... 
[PROGRESS] [Worker #1] => (loops=14000000) (nonce=55999997) (elapsed = 53.4458 seconds) ... 
[PROGRESS] [Worker #3] => (loops=14000000) (nonce=55999999) (elapsed = 54.087 seconds) ... 
[PROGRESS] [Worker #0] => (loops=14000000) (nonce=55999996) (elapsed = 54.3064 seconds) ... 
[PROGRESS] [Worker #2] => (loops=14000000) (nonce=55999998) (elapsed = 55.0472 seconds) ... 
*** Block #8 added to blockchain!

  Block #8 {
    previous_hash => 00000017adea336ebc16be447b82e985cd9af0ab4a3def6ca40bedb294d2a3e1
    current_hash  => 000000cae94e8a875679e58ce20e50aca4d190ac4a92eb3e6af6566ef3e0ac76
    nonce => 70650403
    data (1 entries) => [
      "Block #8",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #8( in 68.1725 s )
... Mining for block #9...
*** Block #9 added to blockchain!

  Block #9 {
    previous_hash => 000000cae94e8a875679e58ce20e50aca4d190ac4a92eb3e6af6566ef3e0ac76
    current_hash  => 000000bf7e9f242bbadb1188ad2e5bf63d338270ca2aaca8204926d2a3c459f2
    nonce => 3930350
    data (1 entries) => [
      "Block #9",
    ]
  }
!---------------------------------------------------------------!
 << FINISHED: Blockchain::Block #9( in 3.76364 s )
... final blockchain ...

BlockChain : [Catena Test] (difficulty=6)
Block Count => 10

  Block #0 {
    previous_hash => 0000000000000000000000000000000000000000000000000000000000000000
    current_hash  => 000000a2da8a8707c0b5101842b0a67cd34d7c6de7339632f64e70f826a69a4f
    nonce => 36039369
    data (2 entries) => [
      "Genesis_Block",
      "Catena Test",
    ]
  }

  Block #1 {
    previous_hash => 000000a2da8a8707c0b5101842b0a67cd34d7c6de7339632f64e70f826a69a4f
    current_hash  => 00000020673a833ef674e17142f5c8b49f1ccc999a17476dbad93f109499e9f2
    nonce => 26738181
    data (1 entries) => [
      "Block #1",
    ]
  }

  Block #2 {
    previous_hash => 00000020673a833ef674e17142f5c8b49f1ccc999a17476dbad93f109499e9f2
    current_hash  => 0000003344bbd1c977a0be7b92415f313d69f62a2ded3add9e27e9c19be00a3a
    nonce => 17959210
    data (1 entries) => [
      "Block #2",
    ]
  }

  Block #3 {
    previous_hash => 0000003344bbd1c977a0be7b92415f313d69f62a2ded3add9e27e9c19be00a3a
    current_hash  => 0000008fa567de30f20f265263771e5c3d9d8ab1999009c56d16b493fad4237f
    nonce => 13415392
    data (1 entries) => [
      "Block #3",
    ]
  }

  Block #4 {
    previous_hash => 0000008fa567de30f20f265263771e5c3d9d8ab1999009c56d16b493fad4237f
    current_hash  => 0000004d52c0071edc279dcd4caa16c99e0bf81dbb376a7e0435dc4d1453fb48
    nonce => 5662802
    data (1 entries) => [
      "Block #4",
    ]
  }

  Block #5 {
    previous_hash => 0000004d52c0071edc279dcd4caa16c99e0bf81dbb376a7e0435dc4d1453fb48
    current_hash  => 000000ec9e4ed64ae764363ac3ea7e0375b1269424617d51282fcf5699eeb7f0
    nonce => 21183430
    data (1 entries) => [
      "Block #5",
    ]
  }

  Block #6 {
    previous_hash => 000000ec9e4ed64ae764363ac3ea7e0375b1269424617d51282fcf5699eeb7f0
    current_hash  => 000000ffb1ce3cb5330df2422496eb79a08317be1d00a530de17ba8d2aa54900
    nonce => 16679565
    data (1 entries) => [
      "Block #6",
    ]
  }

  Block #7 {
    previous_hash => 000000ffb1ce3cb5330df2422496eb79a08317be1d00a530de17ba8d2aa54900
    current_hash  => 00000017adea336ebc16be447b82e985cd9af0ab4a3def6ca40bedb294d2a3e1
    nonce => 3741020
    data (1 entries) => [
      "Block #7",
    ]
  }

  Block #8 {
    previous_hash => 00000017adea336ebc16be447b82e985cd9af0ab4a3def6ca40bedb294d2a3e1
    current_hash  => 000000cae94e8a875679e58ce20e50aca4d190ac4a92eb3e6af6566ef3e0ac76
    nonce => 70650403
    data (1 entries) => [
      "Block #8",
    ]
  }

  Block #9 {
    previous_hash => 000000cae94e8a875679e58ce20e50aca4d190ac4a92eb3e6af6566ef3e0ac76
    current_hash  => 000000bf7e9f242bbadb1188ad2e5bf63d338270ca2aaca8204926d2a3c459f2
    nonce => 3930350
    data (1 entries) => [
      "Block #9",
    ]
  }

 << FINISHED: ::Catena_Basic_Blockchain( in 177.977 s )

```


## What is in it?

* Basic blockchain implementation (in-memory storage)
* Block mining with difficulty
* Multi-threaded block miners / nonce finder
* Block verification logic

## What's to be done next?

I will work on this off and on as this is a hobby project and a literal weekend hack.

I want to add:

* a peer to peer network with distributed consensus on new blocks
* a sqlite based wallet system
* json/protobuf based serialization and deserialization of blocks

If you find bugs and/or have enhancements fire in a pull request and I'll update the repo.



## Third-party/External Libraries (included in source)

Zedwood.com SHA256 C++ library in third-party/
