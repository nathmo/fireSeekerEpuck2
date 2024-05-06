# Fire Seeker Robot

### Introduction
This robot aim to track "fire" (sheet of paper of red color) and roll over them to extinguish them.
It will also "explore" the room and bounce of wall.
### Code function overview

This is the general overview of the state machine.
The function will be explained below 
```

      ┌─────────────────────────────────────────────────────────┐
      │                                                         │
      │                                                         │
      │                                                         │
      │         ┌─────────┐                                     │
      │         │         │                                     │
      ▼         ▼         │                                     │
┌───────────────────┐     │                                     │
│                   │     │                                     │
│ Move forward slow │     │                                     │
│                   │     │                                     │
└────────┬──────────┘     │                                     │
         │                │                                     │
         │                │                                     │
         │                │                                     │
         │                │                                     │
         ▼                │                                     │
┌──┬─────────────┬──┐     │                                     │
│  │ IF obstable │  │     │                                     │
│  │ on the front│  │     │                                     │
│  │ half circle │  │     │                                     │
└──┴─┬───────┬───┴──┘     │                                     │
     │       │            │                                     │
True │       │ False      │                                     │
     │       └────────────┘                                     │
     │                                                          │
     ▼                                                          │
┌───────────────────┐                                           │
│ turn toward the   │                                           │
│                   │                                           │
│ obstacle          │                                           │
└─────────┬─────────┘                                           │
          │                                                     │
          │                                                     │
          │                                                     │
          │                                                     │
          ▼                                                     │
┌───────────────────┐                                           │
│                   │                                           │
│   get an immage   │                                           │
│                   │                                           │
└─────────┬─────────┘                                           │
          │                                                     │
          │                                                     │
          │                                                     │
          │                                                     │
          ▼                                                     │
┌──┬─────────────┬──┐                                           │
│  │ If fire is  │  │                                           │
│  │ Detected on │  │                                           │
│  │ the image   │  │                                           │
└──┴┬──────────┬─┴──┘                                           │
    │          │ True                                           │
    │          └──────────┐                                     │
    │                     │                                     │
    │                     ▼                                     │
    │          ┌───────────────────┐                            │
    │          │ enable light      │                            │
    │          │ and sound effect  │                            │
    │          │ and roll fast fwd │                            │
    │          └──────────┬────────┘                            │
    │                     │                                     │
    │ False               │                                     │
    │          ┌──────────┘                                     │
    ▼          ▼                                                │
┌───────────────────┐                                           │
│ turn away from    │                                           │
│ the obstacle      ├───────────────────────────────────────────┘
│                   │                                            
└───────────────────┘                                            

```
### File structure
