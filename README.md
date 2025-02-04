# EVM

# EVM Device Protocol

## RAPI protocol (Arduino to OpenEVSE controller communication)

OpenEVSE Remote API (RAPI) is a simple lightweight communications protocol. 
RAPI allows the user/application to:
- Execute Commands
- Get Status
- Change Settings
- Debug
  
RAPI is designed to work in any communications environment including high loss connections such as wireless. 
RAPI provides optional features to ensure reliable communications:
- Command/Response
- Checksum (NONE, SUM, XOR [recommended])
- Sequence ID

Command formats
1. with XOR checksum (recommended)<br />
   \$cc pp pp ...^xk\r
2. with additive checksum (legacy)<br />
   \$cc pp pp ...\*ck\r
3. no checksum (FOR TESTING ONLY! DON'T USE FOR APPS)<br />
   \$cc pp pp ...\r
4. checksum + sequence id (v3.0.0+)<br />
   \$cc pp pp .. :ss^xk\r

**Structure**

\$ = start of RAPI command<br />
cc = 2-letter command<br />
pp = parameters<br />
xk = 2-hex-digit checksum - 8-bit XOR of all characters before '^'<br />
ck = 2-hex-digit checksum - 8-bit sum of all characters before '\*'<br />
ss = optional 2-hex-digit sequence id - response will echo the sequence id so that receiver can verify
that the response matches the command - ss CANNOT be 00, which is reserved as an invalid value<br />
\r = carriage return = 13d = 0x0D<br />

**Response format**:

\$OK [optional parameters] [:ss]^xk\r – success<br />
\$NK [optional parameters] [:ss]^xk\r – failure<br />
Checksum and Sequence ID are only present if send with the command<br />

**[EVSE State](https://github.dev/OpenEVSE/open_evse/blob/master/firmware/open_evse/rapi_proc.h)** - OpenEVSE controller state (1 bytes).

  - Sleeping - no vehicle
  - `0x00` unknown
  - `0x01` State A - not connected
  - `0x02` State B - connected
  - `0x03` State C - charging
  - `0x04` State D - error "vent required"
  - `0x05` Error "diode check failed"
  - `0x06` Error "GFI fault"
  - `0x07` Error "bad ground"
  - `0x08` Error "stuck relay"
  - `0x09` Error "GFI self-test failed"
  - `0x0A` Error "over temperature shutdown"
  - `0x0B` Error "over current shutdown"
  - `0x0E` Error "relay closure fault"
  - `0xFE` Error "waiting for timer"
  - `0xFF` Disabled - vehicle is parked
    <br><br>

---
---


## ESP32 to Arduino communication

### Packet structure

1. Communication convention: 115200 Bps, even parity, 8 data bits, 1 stop bit
2. Command data structure:
   - Synchronization header: 2 Byte. All commands and responses are synchronized with 0xAB and 0xCD headers.
   - Command code: 1 Byte.
   - Data length: 1 Byte.
   - Data: According to the different commands, the length of the data is variable.
   - Cyclic Redundancy Check: 2 Byte. The CRC check is used to check the CRC value of the command, 
     address, and data byte. The low byte of the CRC is sent first, followed by the high byte.
   - End of Packet: 2 Byte. Tail bytes are 0xDC, 0xBA.

## Commands dialog

|             Command             |      Command code      | Data                                                                                                                                                                 | Command direction | Description                                                          | Resp.  |
| :-----------------------------: | :--------------------: | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :---------------: | -------------------------------------------------------------------- | :----: |
| Get Mega<br>Firmware<br>Version | `0x01`<br>(DL: `0x00`) |                                                                                                                                                                      |    ESP -> MEGA    | Get firmware version<br>of Mega                                      | `0x02` |
|   Mega<br>Firmware<br>Version   | `0x02`<br>(DL: `0x03`) | Major (1 byte)<br>Minor (1 byte)<br>Patch (1 byte)                                                                                                                   |    ESP -> MEGA    | Firmware version<br>of Mega                                          |  `-`   |
|          Mega<br>Resp.          | `0xAA`<br>(DL: `0x02`) | Command Id (1 byte),<br>Ack (1 byte):<br> - 0x00: Success<br> - 0xee: Unknown                                                                                        |    MEGA -> ESP    | Ack commands                                                         |  `-`   |
|              Print              |  `0x34`<br>(DL: `~`)   | Command Id (1 byte),<br>Row 1 DL (1 byte)<br>Text 1 (Row 1 DL bytes)<br>Row 2 DL (1 byte)<br>Text 2 (Row 2 DL bytes)<br>Row 3 DL (1 byte)<br>Text 3 (Row 3 DL bytes) |    ESP -> MEGA    | Send some text to print<br>on LT-3XS664-595                          | `0xAA` |
|          Print<br>Row           |  `0x35`<br>(DL: `~`)   | Command Id (1 byte),<br>Row Index (1 byte)<br>Text (DL bytes)                                                                                                        |    ESP -> MEGA    | Send some text to print<br>on a specific row (0-2)                   | `0xAA` |
|    Print<br>Animated<br>Row     |  `0x36`<br>(DL: `~`)   | Command Id (1 byte),<br>Row Index (1 byte)<br>Text (DL bytes)<br>Text Animation (1 byte)<br>Animation Time (2 bytes)                                                 |    ESP -> MEGA    | Send some text to print<br>on a specific row (0-2)<br>with animation | `0xAA` |
|       Control<br>Barrier        | `0x46`<br>(DL: `0x02`) | Command Id (1 byte),<br>Command (1 byte):<br> - 0x00: close<br> - 0x01: open                                                                                         |    ESP -> MEGA    | Open or Close the barrier                                            | `0xAA` |
|       Control<br>LED Ring       | `0x57`<br>(DL: `0x07`) | Command Id (1 byte),<br>Color (3 bytes)<br>Animation (1 byte)<br>Animation Time (2 bytes)                                                                            |    ESP -> MEGA    | Control Led Ring                                                     | `0xAA` |
|   Card <br>Reader<br>Request    | `0x68`<br>(DL: `0x01`) | Command Id (1 byte)                                                                                                                                                  |    ESP -> MEGA    |                                                                      | `0x39` |
|   Card<br>Reader<br>Response    | `0x69`<br>(DL: `0x05`) | Command Id (1 byte),<br>Card UID (4 bytes)                                                                                                                           |    MEGA -> ESP    |                                                                      |  `-`   |
|        Get Pin<br>State         | `0x90`<br>(DL: `0x02`) | Command Id (1 byte),<br>Pin Number (1 byte)                                                                                                                          |    ESP -> MEGA    | Get state of pin                                                     | `0x93` |
|        Set Pin<br>State         | `0x91`<br>(DL: `0x03`) | Command Id (1 byte),<br>Pin Number (1 byte)<br>Pin State (1 byte)                                                                                                    |    ESP -> MEGA    | Set OUTPUT mode and<br>set state of pin                              | `0x93` |
|         Observe<br>Pin          | `0x92`<br>(DL: `0x02`) | Command Id (1 byte),<br>Pin Number (1 byte)                                                                                                                          |    ESP -> MEGA    | Set INPUT mode and<br>notify any state change                        | `0x93` |
|          Pin<br>State           | `0x93`<br>(DL: `0x02`) | Command Id (1 byte),<br>Pin State (1 byte)                                                                                                                           |    MEGA -> ESP    | state of pin (HIGH or LOW)                                           |  `-`   |


## Packet data description

<br>

- **Command Id** - Unique id of command (if optional, fill with `0xFF`)
- **Row N DL** - Length text to print on LCD
- **Card UID** - EPass card UID number (4 byte).
- **Animation** - Led ring animation  (1 byte):
  - OFF: `0x00`
  - ON: `0x01`
  - FADE: `0x02`
  - BLINK: `0x03`
  - CIRCLE: `0x04`
    <br>
- **Text Animation** - LCD text animation (1 byte):
  - NONE: `0x05`
  - BLINK: `0x06`
  - TIMED: `0x07`
  - RUNNING_LINE: `0x08`
- **Animation Time** - Animation time in ms (2 bytes). If the animation is set to `ON` and the 
  `Animation Time` is set to `0x0000`, the LED Ring will remain permanently turned on.

---

---

## ESP32 to Backend

```ts
// topic:
"evm.device.361FB608-08:B6:1F:36:48:00.<COMMAND>"

// DEVICE TO SERVER
// command: INITIALIZE
{
  versionESP: string    // X.X.X
  versionMEGA: string   // X.X.X
  ping: number
  price: number
  waitingPrice: number
}

// resp:
{ ack: true }


// command: RESTORE_STATE
{
  options: undefined //for future
}

// resp:
{
  status: 
        | 'Idle'
        | 'Wait'
        | 'Charge'
        | 'Payment'
        | 'Error'
        | 'Repair'
}


// command: CHANGE_STATE
{
  commandId: string | undefined
  time: number
  status: 
        | 'Idle'
        | 'Wait'
        | 'Charge'
        | 'Payment'
        | 'Error'
        | 'Repair'
  reason:
        | 'App.Started'
        | 'App.Finished'
        | 'Car.OnSpot'
        | 'Car.Charged'
        | 'Car.Plugged'
        | 'Car.Unplugged'
        | 'Payment.Succeed'
        | 'Payment.Failed'
        | 'Server.Control'
        | 'Server.Restore'
        | 'Server.QRRead'
        | 'Device.EVSE_NO_RESPONSE'
        | 'Device.MEGA_NO_RESPONSE'
        | 'Device.Error',
        | 'Device.CardRead'
  customerOId: string | undefined
}


// command: CHARGE,
{
  time: number,
  energy: number // Wh
}

// resp:
{ ack: true }


// command: MONITOR,
{
  commandId: string | undefined
  time: number
  ping: number
  price: number
  waitingPrice: number
}


// FROM SERVER TO DEVICE
// command: SET_PRICE
{
  commandId: string
  price: number
  waitingPrice: number
}
// resp: emit MONITOR


// command: START
{
  commandId: string
  customerOId: string
}
// resp: emit CHANGE_STATE


// command: FINISH
{
  commandId: string
  customerOId: string
}
// resp: emit CHANGE_STATE


// command: PAYMENT
{
  commandId: string
  status: 'success'| 'fail'
}
// resp: emit CHANGE_STATE


// command: QR_READ
{
  commandId: string
  msg: string
}
// resp: emit CHANGE_STATE

// command: RESTART
{
  options: undefined //for future
}
// resp:
{ 
  msg: 'restarting' 
  time: number
}


// command: UPDATE
{
  url: string,
  md5: string,
}
// resp:
{
  msg: 'updating'
  time: number
}
```

## The Graph of States:

![Device States](/device/docs/state_chart.png)

```ts
[
  [
    'Idle',
    new Map([
      ['Car.OnSpot', 'Wait'],
      ['App.Started', 'Wait'],
      ['App.Started', 'Charge'],
      ['Car.Plugged', 'Wait'],
      ['Device.Error', 'Idle'],
      ['Device.EVSE_NO_RESPONSE', 'Error'],
      ['Device.MEGA_NO_RESPONSE', 'Error'],
    ]),
  ],
  [
    'Wait',
    new Map([
      ['App.Started', 'Charge'],
      ['Car.Plugged', 'Charge'],
      ['App.Started', 'Wait'],
      ['Car.Plugged', 'Wait'],
      ['Device.CardRead', 'Charge'],
      ['Device.CardRead', 'Payment'],
      ['App.Finished', 'Payment'],
      ['Server.QRRead', 'Payment'],
      ['Device.Error', 'Wait'],
      ['Payment.Failed', 'Wait'],
      ['Payment.Succeed', 'Idle'],
      ['Device.EVSE_NO_RESPONSE', 'Error'],
      ['Device.MEGA_NO_RESPONSE', 'Error'],
    ]),
  ],
  [
    'Charge',
    new Map([
      ['App.Finished', 'Payment'],
      ['Device.CardRead', 'Payment'],
      ['Car.Unplugged', 'Wait'],
      ['Car.Charged', 'Wait'],
      ['Server.QRRead', 'Payment'],
      ['Device.Error', 'Charge'],
      ['Device.EVSE_NO_RESPONSE', 'Error'],
      ['Device.MEGA_NO_RESPONSE', 'Error'],
    ]),
  ],
  [
    'Payment',
    new Map([
      ['Payment.Succeed', 'Idle'],
      ['Payment.Failed', 'Wait'],
      ['Device.Error', 'Payment'],
      ['Device.Error', 'Wait'],
      ['App.Finished', 'Payment'],
      ['Device.EVSE_NO_RESPONSE', 'Error'],
      ['Device.MEGA_NO_RESPONSE', 'Error'],
    ]),
  ],
]
```

---
