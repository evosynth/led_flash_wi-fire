<!---
Copyright 2017 Julian Ingram

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
--->

# LED Flash Project for the chipKIT Wi-FIRE

Made to compile with the [Imagination Technologies toolchain](http://community.imgtec.com/developers/mips/tools/codescape-mips-sdk/download-codescape-mips-sdk-essentials/). Select the "Bare Metal" and "MIPS Warrior Series 5 CPU" options when installing.

Compiling should just be a case of executing `make` in the root dir. The .elf and .hex files should then be found in the /bin dir. The .hex file can be uploaded using MPLAB IPE and a PICkit or similar.

After a succesful upload, LED 1 should toggle once per second.
