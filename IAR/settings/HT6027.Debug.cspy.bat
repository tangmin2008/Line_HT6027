@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


"D:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0_2\common\bin\cspybat" "D:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0_2\arm\bin\armproc.dll" "D:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0_2\arm\bin\armjlink.dll"  %1 --plugin "D:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0_2\arm\bin\armbat.dll" --flash_loader "D:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0_2\arm\config\flashloader\Hitrendtech\HT6X3X.board" --backend -B "--endian=little" "--cpu=Cortex-M0" "--fpu=None" "-p" "D:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0_2\arm\CONFIG\debugger\Hitrendtech\HT6X3X.SVD" "--drv_verify_download" "--semihosting=none" "--device=HT6X3X" "--drv_communication=USB0" "--drv_interface_speed=auto" "--jlink_initial_speed=100" "--jlink_reset_strategy=0,0" "--drv_interface=SWD" "--drv_catch_exceptions=0x000" "--drv_swo_clock_setup=22000000,1,1000000" 


