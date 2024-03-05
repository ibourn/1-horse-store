// bytecode at compilation : 603680600a5f395ff3fe5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd
// runtime bytecode : 5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd
// Contract creation
object "HorseStoreYul" {
    code {
        // Contract deployment
        // eqivalent to codecopy => stick with 0 offset in memory with the size of the runtime code
        datacopy(0, dataoffset("runtime"), datasize("runtime"))
        //return from the memory from 0 to the size of the runtime code
        return(0, datasize("runtime"))
    }
    object "runtime" {
        code {
            // Function dispatcher
            switch selector()
            //updateHorseNumber
            case 0xcdfead2e {
                storeNumber(decodeAsUint(0))
            }
            // readNumberOfHorses
            case 0xe026c017 {
                returnUint(readNumber())
            }
            default {
                // Invalid function selector
                revert(0, 0)
            }

            /*-- Storage functions --*/
            function storeNumber(newNumber) {
                // check free memory pointer if there's more variables
                sstore(0, newNumber)
            }

            function readNumber() -> storedNumber {
                storedNumber := sload(0)
            }

            /*-- Decodgin functions --*/
            function selector() -> s {
                // Load the first 4 bytes of the calldata  
                s := div(calldataload(0), 0x100000000000000000000000000000000000000000000000000000000)
            }

            function decodeAsUint(offset) -> v {
                // Which is the position in the calldata (passing the selector)
                let positionInCalldata := add(4, mul(offset, 0x20))
                // Equivalent to size checking (it should be at least 32 bytes + selector)
                if lt(calldatasize(), add(positionInCalldata, 0x20)) {
                    revert(0, 0)
                }
                v := calldataload(positionInCalldata)
            }

            function returnUint(v) {
                mstore(0, v)
                return(0, 0x20)
            }
        }
    }

}