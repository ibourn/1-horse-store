// SPDX-License-Identifier: MIT

pragma solidity 0.8.20;

import {Base_TestV1, IHorseStore} from "./Base_TestV1.t.sol";
import {HuffDeployer} from "foundry-huff/HuffDeployer.sol";

contract HorseStoreHuff is Base_TestV1 {
    // it assumes all is in src and that's a huff file
    string public constant HUFF_SOURCE = "horseStoreV1/HorseStore";

    function setUp() public override {
        // super.setUp();
        horseStore = IHorseStore(HuffDeployer.config().deploy(HUFF_SOURCE));
    }
}
