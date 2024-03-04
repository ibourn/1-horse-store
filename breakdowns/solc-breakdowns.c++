//0x6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220ee718384a20a40f58b676e7f7ba7521f229a5f787acec88f60bea1052b0d150d64736f6c63430008140033

// 1. Contract Creation Code
// Free memory pointer is 0x40 // 0x00 0x20 reserved to hash stuff (slot 32 bytes => 0x20 in hex)
// send to 0x80 where memory not reserved
// huff has not free memory pointer
PUSH1 0x80                  // stack : [0x80]
PUSH1 0x40                  // stack : [0x80, 0x40]
MSTORE                      // stack : []           / memory : 0x40 => [0x80]


// if someone send ether to the contract : REVERT, else : jump to 0x0E PC/JUMPDEST
// adding a constructor payable skip this check so save gas
CALLVALUE                   // stack : [msg.value]
DUP1                        // stack : [msg.value, msg.value]
ISZERO                      // stack : [msg.value == 0, msg.value]
PUSH1 0x0e                  // stack : [0x0E, msg.value == 0, msg.value]
JUMPI                       // stack : [msg.value] if we don't jump

PUSH0                       // stack : [0x0, msg.value]
DUP1                        // stack : [0x0, 0x0, msg.value]
REVERT                      // stack : [msg.value] 


// jumpdest if msg.value == 0
// sticks the runtime bytecode on-chain
// to create contract : Create, create2 OR sending nil in the 'To' field of a transaction
JUMPDEST                    // stack : [msg.value]
POP                         // stack : []
PUSH1 0xa5                  // stack : [0xA5]
DUP1                        // stack : [0xA5, 0xA5]
PUSH2 0x001b                // stack : [0x001B, 0xA5, 0xA5]
PUSH0                       // stack : [0x0, 0x001B, 0xA5, 0xA5]
// copy binary data from 0x001B to 0x001B + 0xA5 (only the runtime bytecode) / 0x0 is destination offset (we have no offset there)
CODECOPY                    // stack : [0xA5]           / memory : 0x0 => [runtime bytecode]
PUSH0                       // stack : [0x0, 0xA5]
RETURN                      // stack : []               / memory : 0x0 => [runtime bytecode]
INVALID                     // stack : []               / memory : 0x0 => [runtime bytecode]


// 2. Runtime Bytecode
// Entry point of our calls
// free memory pointer 
PUSH1 0x80 
PUSH1 0x40
MSTORE

// Checks for msg.value & reverts if not 0
// compiler see that no function are payable so it checks before checking the function if msg.value == 0
CALLVALUE                  // stack : [msg.value]
DUP1                       // stack : [msg.value, msg.value]
ISZERO                     // stack : [msg.value == 0, msg.value]
PUSH1 0x0e                 // stack : [0x0E, msg.value == 0, msg.value]
JUMPI                      // stack : [msg.value] if we don't jump
PUSH0                      // stack : [0x0, msg.value]
DUP1                       // stack : [0x0, 0x0, msg.value]
REVERT                     // stack : [msg.value]

// jumpdest if msg.value == 0
// Checks if calldatasize < 0x04 so if it's long enough to contain the function selector
JUMPDEST                   // stack : [msg.value]
POP                        // stack : []
PUSH1 0x04                 // stack : [0x04]
CALLDATASIZE               // stack : [calldatasize, 0x04]
LT                         // stack : [calldatasize < 0x04]
PUSH1 0x30                 // stack : [0x30, calldatasize < 0x04] , 0x30 is the PC of the JUMP
JUMPI                      // stack : [] 
// calldata jump to 0x30 ('calldata_jump') if call data size is long enough

// Funciton dispatcher
PUSH0                      // stack : [0x0]
CALLDATALOAD               // stack : [calldata]
PUSH1 0xe0                 // stack : [0xE0, calldata] (224 bits == 28 bytes, calldata truncated to 32 first bytes)
SHR                        // stack : [calldata >> 0xE0] -> functionSelector
// Dispatching to the updateHorseNumber
DUP1                       // stack : [functionSelector , functionSelector] 
PUSH4 0xcdfead2e           // stack : [0xCDFEAD2E, functionSelector , functionSelector]
EQ                         // stack : [functionSelector == 0xCDFEAD2E, functionSelector]
PUSH1 0x34                 // stack : [0x34, functionSelector == 0xCDFEAD2E, functionSelector]
JUMPI                      // stack : [functionSelector] if we don't jump
// Dispatching to the readNumberOfHorses
DUP1                       // stack : [functionSelector, functionSelector]  
PUSH4 0xe026c017           // stack : [0xE026C017, functionSelector, functionSelector]
EQ                         // stack : [functionSelector == 0xE026C017, functionSelector]
PUSH1 0x45                 // stack : [0x45, functionSelector == 0xE026C017, functionSelector]
JUMPI                      // stack : [functionSelector] if we don't jump


// If no function selector match, no jump, comes here and revert
// calldata_jump => REVERT jumpDest
JUMPDEST                   // stack : []
PUSH0                      // stack : [0x0]
DUP1                       // stack : [0x0, 0x0]
REVERT                     // stack : [0x0]

// updateHorseNumber jump dest 1
JUMPDEST                   // stack : [functionSelector] 
PUSH1 0x43                 // stack : [0x43, functionSelector]
PUSH1 0x3f                 // stack : [0x3F, 0x43, functionSelector]
CALLDATASIZE               // stack : [calldatasize, 0x3F, 0x43, functionSelector]
PUSH1 0x04                 // stack : [0x4, calldatasize, 0x3F, 0x43, functionSelector] 
PUSH1 0x59                 // stack : [0x59, 0x4, calldatasize, 0x3F, 0x43, functionSelector]
// Jump to the Program Counter (last in stack)
JUMP

// readNumberOfHorses jump dest 4
// We run a sstore to save our horse number
// dispatcher -> msg.value checks -> calldatasize checks -> extraction of the number from the calldata
JUMPDEST                   // stack : [calldata, 0x43, functionSelector]
PUSH0                      // stack : [0x0, calldata, 0x43, functionSelector]
SSTORE                     // stack : [calldata, 0x43, functionSelector]
JUMP                       // stack : [0x43, functionSelector]
// jump to jump dest 5 (0x43)

// updateHorseNumber jump dest 5
JUMPDEST                   // stack : [functionSelector]
STOP                       // stack : [functionSelector]

// readNumberOfHorses jump dest 1
// Read the horse number from storage and return it
JUMPDEST                  // stack : [functionSelector]
PUSH0                     // stack : [0x0, functionSelector]
SLOAD                     // stack : [horseNumber, functionSelector] 
PUSH1 0x40                // stack : [0x40, horseNumber, functionSelector]
MLOAD                     // stack : [0x80, horseNumber, functionSelector] (memory was : 0x40 => [0x80])
SWAP1                     // stack : [horseNumber, 0x80, functionSelector]
DUP2                      // stack : [horseNumber, 0x80, 0x80, functionSelector]
MSTORE                    // stack : [0x80, functionSelector]    / memory : [0x40 => [0x80] , 0x80 => [horseNumber]]
PUSH1 0x20                // stack : [0x20, 0x80, functionSelector] => load the size of the horse number (32 bytes)
ADD                       // stack : [0xA0, functionSelector] => add the offset to the horse number to get the end of the horse number
PUSH1 0x40                // stack : [0x40, 0xA0, functionSelector]
MLOAD                     // stack : [0x80, 0xA0, functionSelector]
DUP1                      // stack : [0x80, 0x80, 0xA0, functionSelector]
SWAP2                     // stack : [0xA0, 0x80, 0x80, functionSelector]
SUB                       // stack : [0x20, 0x80, functionSelector] => get the size of data to return
SWAP1                     // stack : [0x80, 0x20, functionSelector]
// return the horse number (memory offset 0x80 (= location), till offset + length 0x20)
RETURN

// updateHorseNumber jump dest 2
// Checks if there is a value to update the horse number
JUMPDEST                   // stack : [0x04, calldatasize, 0x3F, 0x43, functionSelector]
PUSH0                      // stack : [0x0, 0x04, calldatasize, 0x3F, 0x43, functionSelector]
PUSH1 0x20                 // stack : [0x20, 0x0, 0x04, calldatasize, 0x3F, 0x43, functionSelector]
DUP3                       // stack : [0x04, 0x20, 0x0, 0x04, calldatasize, 0x3F, 0x43, functionSelector]
DUP5                       // stack : [calldatasize, 0x04, 0x20, 0x0, 0x4, calldatasize, 0x3F, 0x43, functionSelector]
SUB                        // stack : [calldatasize - 0x04, 0x20, 0x0, 0x4, calldatasize, 0x3F, 0x43, functionSelector]
// signed less than comparison
// is there more calldata than just the function selector ?
SLT                        // stack : [calldatasize - 0x04 < 0x20, 0x0, 0x4, calldatasize, 0x3F, 0x43, functionSelector]
ISZERO                     // stack : [(calldatasize - 0x04 < 0x20) == 0, 0x0, 0x4, calldatasize, 0x3F, 0x43, functionSelector]
PUSH1 0x68                 // stack : [0x68, (calldatasize - 0x04 < 0x20) == 0, 0x0, 0x4, calldatasize, 0x3F, 0x43, functionSelector] 
JUMPI
// jump to jump dest 3 if there is more calldata than just the function selector
// Rever if there is not enough calldata
PUSH0
DUP1
REVERT

// updateHorseNumber jump dest 3
// Grab the calldata for the horse number and delete things from the stack
JUMPDEST                  // stack : [0x0, 0x4, calldatasize, 0x3F, 0x43, functionSelector]
POP                       // stack : [0x4, calldatasize, 0x3F, 0x43, functionSelector]
// Ignore the function selector and grab the data
CALLDATALOAD              // stack : [calldata, calldatasize, 0x3F, 0x43, functionSelector]
SWAP2                     // stack : [0x3F, calldatasize, calldata, 0x43, functionSelector]
SWAP1                     // stack : [calldatasize, 0x3F, calldata, 0x43, functionSelector]
POP                       // stack : [0x3F, calldata, 0x43, functionSelector]
JUMP                      // stack : [calldata, 0x43, functionSelector]
// jump to jump dest 4 (0x3F)


// 3. Metadata
// no tx can go there cause there's no jumpdest
// solidity put there info about the compiler version and the source code hash... tool like etherscan make verification ...
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
SHA3
INVALID
PUSH18 0x8384a20a40f58b676e7f7ba7521f229a5f78
PUSH27 0xcec88f60bea1052b0d150d64736f6c63430008140033