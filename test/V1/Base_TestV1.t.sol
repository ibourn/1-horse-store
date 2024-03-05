// SPDX-License-Identifier: MIT

pragma solidity 0.8.20;

import {Test, console2} from "forge-std/Test.sol";
import {HorseStore} from "../../src/horseStoreV1/HorseStore.sol";
import {IHorseStore} from "../../src/horseStoreV1/IHorseStore.sol";

abstract contract Base_TestV1 is Test {
    IHorseStore horseStore;

    function setUp() public virtual {
        horseStore = IHorseStore(address(new HorseStore()));
    }

    function test_ReadValue() public {
        uint256 value = horseStore.readNumberOfHorses();
        console2.log("Value: %s", value);
        assertEq(value, 0);
    }

    function test_WriteValue(uint256 numberOfHorses) public {
        // uint256 numberOfHorses = 5;
        horseStore.updateHorseNumber(numberOfHorses);
        uint256 value = horseStore.readNumberOfHorses();
        console2.log("Value: %s", value);
        assertEq(value, numberOfHorses);
    }
}
