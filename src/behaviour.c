/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : behaviour.c
 * 
 * This file contains the high level logic of the state machine of the robot.
 * 
 * TODO : implement a thread to modele that state machine and pass the state as semaphore to other thread.  (Nathann Morand)
 * 
      ┌─────────────────────────┬───────────────────────────────┐
      │                         │                               │
      │                         │                               │
      │                         │                               │
      │         ┌─────────┐     │                               │
      │         │         │     │                               │
      ▼         ▼         │     │                               │
┌───────────────────┐     │     │       ┌──────────┐            │
│                   │     │     │       │          │            │
│ Move forward slow │     │     │       │          │            │
│                   │     │     │false  │ true     │            │
└────────┬──────────┘     │     │       │          │            │
         │                │     │       │          │            │
         │                │ ┌──┬┴───────┴────┬──┐  │            │
         │                │ │  │ IF obstable │  │  │            │
         │                │ │  │ on the front│  │  │            │
         ▼                │ │  │ half circle │  │  │            │
┌──┬─────────────┬──┐     │ └──┴─────────────┴──┘  │            │
│  │ IF obstable │  │     │           ▲            │            │
│  │ on the front│  │     │           │            │            │
│  │ half circle │  │     │           │            │            │
└──┴─┬───────┬───┴──┘     │           │            │            │
     │       │            │           │            ▼            │
True │       │ False      │           │  ┌──┬─────────────┬──┐  │
     │       └────────────┘           │  │  │If time out  │  │  │
     │                                │  │  │             │  │  │
     ▼                                │  │  │occured      │  │  │
┌───────────────────┐                 │  └──┴─┬───────┬───┴──┘  │
│ turn toward the   │                 │       │       │ True    │
│                   │                 │       │       │         │
│ obstacle          │                 │       │       │         │
└─────────┬─────────┘                 │       │       │         │
          │                           │       │ False │         │
          │                           │       │       │         │
          │                           │       └────┐  │         │
          │                           │            │  │         │
          ▼                           │            │  │         │
┌───────────────────┐       ┌─────────┴─────────┐  │  │         │
│                   │       │ enable light      │  │  │         │
│   get an immage   │       │ and sound effect  │◄─┘  │         │
│                   │       │ and roll fast fwd │     │         │
└─────────┬─────────┘       └───────────────────┘     │         │
          │                           ▲               │         │
          │                           │               │         │
          │                           │               │         │
          │                           │               │         │
          ▼                           │               │         │
┌──┬─────────────┬──┐                 │               │         │
│  │ If fire is  │  │                 │               │         │
│  │ Detected on │  │                 │               │         │
│  │ the image   │  │                 │               │         │
└──┴┬──────────┬─┴──┘                 │               │         │
    │          │ True                 │               │         │
    │          └──────────────────────┘               │         │
    │ False                                           │         │
    │          ┌──────────────────────────────────────┘         │
    ▼          ▼                                                │
┌───────────────────┐                                           │
│ turn away from    │                                           │
│ the obstacle      ├───────────────────────────────────────────┘
│                   │                                            
└───────────────────┘                                            
*/