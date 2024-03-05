// SPDX-License-Identifier: MIT

pragma solidity 0.8.20;

import {Base_TestV1, IHorseStore} from "./Base_TestV1.t.sol";
// import {HorseStoreYul} from "../../src/horseStoreV1/HorseStoreYul.sol";

contract HorseStoreRawYulTest is Base_TestV1 {
    function setUp() public override {
        horseStore = IHorseStore(createContract());
    }

    function createContract() public returns (address) {
        bytes memory yulCode =
            hex"603680600a5f395ff3fe5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd";
        // runtime bytecode : hex"5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd";

        address horseStoreAddress;
        assembly {
            horseStoreAddress := create(0, add(yulCode, 0x20), mload(yulCode))
        }

        require(horseStoreAddress != address(0), "YulDeployer could not deploy contract");
        return horseStoreAddress;
    }
}
