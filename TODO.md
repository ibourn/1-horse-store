# Horse Store

1. basic simplestorage/horse store

// call to HorseStore.updateHorseNumber(uint256) via remix
// observe calldata

// 1. where this data comes from

first bytes : 0xcdfead2e = function selector (4 bytes)

The EVM reads the instructions and presents this selector
The function selector of the contract compares it to its selectors and allows to jump to the corresponding code

=> solidity compiler do the function dispatcher for us, not yul & huff
it allows optimization but it skips security offered by solidity compiler

=> it's the entry point of the smart contract to know what to do next

there's function... but for now, use of macros
Huff need a main() function

// 2. How does remix know how to update horse number whith this data
