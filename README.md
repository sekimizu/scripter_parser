# scripter_parser
Check md5sum and content of configuration between two files

## Goals

Compare the given two files, one is made in hex-format, and the other one is modified config file.


## Key file part

The key file should be exactlly the same with config file at first.
It indicates some un-changable cmd and value.

To avoid modification, it will be encrypt into hex format and calculate a MD5SUM.

__xxxx__=__yyyy__ 

__xxxx__(__yyyy__) 

__xxxx__.sh __yyyy__ 


And also, there is '#' for comment, or the space may be presented.

For thus command that could be overwrite by config file will mark '*' in the frist position of each line
Otherwise, all variables could not be changed.


## Config file part

Same content with key file, but stored in ASCII format.
And also, it will record a MD5sum in any line of this file.

`##_<MD5SUM>`

For example: __## cadc076e0f36c0b2cc5355f71cd977c5__

The program will record this variable, and calculate 'key' file's md5sum.
This two values should be the same(w/o modification by user)


## Check content

Act like diff function.
The program will check the number of each command that appear in key file.
And also it's value for each command.

And then, do the comparison with config file, these command which mark '*' will be ignored.
Returns {1, 0, -1} if {pass, failed, error case}


## How to use

### 1. ScripterParser.exe <config_file>

It will generate a key file in the same folder in hex format. (using openssl/md5)
And display the MD5sum in stdout.

`C:\Users\kenny.pan\Desktop\scripter_checker>C:\Users\kenny.pan\Desktop\scripter_checker\ScripterParser.exe C:\Users\kenny.pan\Desktop\SM-All_Test_sys_sh.txt

 Start transfer text to hex data key...
.....................................................................................................
.....................................................................................................
 MD5 = 3f2c67643f30f7b9ffb045c9388f65c2`

### 2. ScripterParser.exe <key_file> <config_file>


C:\Users\kenny.pan\Desktop\scripter_checker>C:\Users\kenny.pan\Desktop\scripter_checker\ScripterParser.exe C:\Users\kenny.pan\Desktop\key C:\Users\kenny.pan\Desktop\SM-All_Test_sys_sh.txt
<br>
 >>>>>> Read key<br>
 >>>>>> Read config<br>
 Get key's md5sum = [cadc076e0f36c0b2cc5355f71cd977c5]<br>
 >>>>>> Check md5sum<br>
 >>>>>> Start compare<br>
 Check CMD : Check_IMEI_number<br>
 Check CMD : LOCAL_DISK<br>
 Check CMD : KEYWORD<br>
 Check CMD : POWER_PORT<br>
 Check CMD : ERROR_HANDLE<br>
 Check CMD : POWER_CTRL<br>
 Check CMD : TIMEOUT<br>
 Check CMD : CONSOLE_PORT<br>
 Check CMD : IMU_test<br>
 Check CMD : LOG_IP<br>
 Check CMD : LOG_TESTMODE<br>
 Check CMD : WAV_Play_R<br>
 Check CMD : SBC_BootHWID_Check<br>
 Check CMD : check_FFC_FPS<br>
 Check CMD : LOCAL_OTG_DISK<br>
 Check CMD : SBC_SPI_2_Check<br>
 Check CMD : PART_NO<br>
 Check CMD : Product_Version_FFC<br>
 Check CMD : DFC_Test<br>
 Check CMD : SBC_MCode_Check<br>
 Check CMD : Get_ADC_Power_in_MON<br>
 Check CMD : FFC_Test<br>
 Check CMD : Check_Bluetooth_Mac<br>
 Check CMD : SBC_Test<br>
 Check CMD : SBC_SWITCH_BIOS_2_CHECK<br>
 Check CMD : Get_ADC_3v3_MON<br>
 Check CMD : Product_Version_SBC<br>
 Check CMD : Check_FFC_image_init<br>
 Check CMD : Date_Code_SBC<br>
 Check CMD : SD_test<br>
 Check CMD : Product_Version_DFC<br>
 Check CMD : Date_Code_DFC<br>
 Check CMD : Date_Code_FFC<br>
 Check CMD : PWM_test<br>
 Check CMD : PicBox<br>
 Check CMD : Write_DateCode<br>
 Check CMD : Linux_Cmd<br>
 Check CMD : Get_Piezo_ADC<br>
 Check CMD : Delay<br>
 Check CMD : 3G_RF_test_3gnet<br>
 Check CMD : SBC_SPI_1_Check<br>
 Check CMD : confirm<br>
 Check CMD : WAV_Play_L<br>
 Check CMD : Write_Check_DFCInfo<br>
 Check CMD : check_DFC_FPS<br>
 Check CMD : Boardinfo<br>
 Check CMD : SIM_CARD<br>
 Check CMD : PassFailPic<br>
 Check CMD : SW2_test<br>
 Check CMD : MCU_VER_Check<br>
 Check CMD : PassFailMsg<br>
 Check CMD : MCU_Power1_test<br>
 Check CMD : temperature_test<br>
 Check CMD : Get_IRLED_Driver_ADC<br>
 Check CMD : Check_LK_BIOS<br>
 Check CMD : rec_audio_R<br>
 Check CMD : CheckSemaInfo<br>
 Check CMD : Get_ADC_LIGHTBAR_HEALTH<br>
 Check CMD : SBC_external_GPS<br>
 Check CMD : MPU9250_SeltTest_IRboard_IMU<br>
 Check CMD : flashfirmware_with_88_debug<br>
 Check CMD : bluetooth_test<br>
 Check CMD : eMMC_test<br>
 Check CMD : light_sensor<br>
 Check CMD : Write_Check_FFCInfo<br>
 Check CMD : SBC_MCode_HWID_Check<br>
 Check CMD : Write_BiosInfo<br>
 Check CMD : SBC_MBoot_Check<br>
 Check CMD : Driver_MCode_Check<br>
 Check CMD : Driver_MCode_HWID_Check<br>
 Check CMD : Driver_MBoot_Check<br>
 Check CMD : Driver_BootHWID_Check<br>
 All match !!!<br>
 <br>
C:\Users\kenny.pan\Desktop\scripter_checker>













