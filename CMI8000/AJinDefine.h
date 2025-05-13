///////////////////////////////////////////////////////////////////////////////
// AJinDefine.h : AJin Structure
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Motion Axis
const int AXIS_COUNT = 46;		// Motion Module

const int AX_LOAD_STAGE_X		= 0;
const int AX_LOAD_STAGE_Z		= 1;
const int AX_LOAD_PICKER_Y		= 2;
const int AX_ANGLE_UNIT_Y		= 3;
const int AX_ANGLE_UNIT_X		= 4;
const int AX_ANGLE_UNIT_R		= 5;
const int AX_VISION_ANGLE_Z		= 6;
const int AX_VISION_ALIGN_Z		= 7;
const int AX_ANGLE_STAGE1_Y		= 8;
const int AX_ANGLE_STAGE1_Z		= 9;
const int AX_ANGLE_STAGE2_Y		= 10;
const int AX_ANGLE_STAGE2_Z		= 11;
const int AX_BTM1_PICKER_X		= 12;
const int AX_BTM1_PICKER_Z		= 13;
const int AX_BTM1_PICKER_P1		= 14;
const int AX_BTM1_PICKER_P2		= 15;
const int AX_BTM1_SHIFT_Y		= 16; //삭제됨
const int AX_INSPECT_STAGE1_X	= 17;
const int AX_INSPECT_STAGE2_X	= 18;
const int AX_INSPECT_STAGE3_X	= 19;
const int AX_MODULE_ALIGN_Y		= 20;
const int AX_MODULE_ALIGN_Z		= 21;
const int AX_VISION_TOP1_Z		= 22;
const int AX_TOP1_MIRROR_Z		= 23;
const int AX_TOP2_SHIFT_Y		= 24;
const int AX_VISION_TOP2_Z		= 25;
const int AX_BTM2_PICKER_X		= 26;
const int AX_BTM2_PICKER_Z		= 27;
const int AX_BTM2_PICKER_P1		= 28;
const int AX_BTM2_PICKER_P2		= 29;
const int AX_BUFFER_STAGE1_Y	= 30;
const int AX_BUFFER_STAGE2_Y	= 31;
const int AX_SORT_PICKER1_X		= 32;
const int AX_SORT_PICKER1_Z		= 33;
const int AX_SORT_PICKER1_P		= 34;
const int AX_SORT_PICKER2_X		= 35;
const int AX_SORT_PICKER2_Z		= 36;
const int AX_SORT_PICKER2_P		= 37;
const int AX_GOOD_STAGE1_Y		= 38;
const int AX_GOOD_STAGE1_Z		= 39;
const int AX_GOOD_STAGE2_Y		= 40;
const int AX_GOOD_STAGE2_Z		= 41;
const int AX_NG_STAGE_Y			= 42;
const int AX_EMPTY_TRANS1_X		= 43;
const int AX_EMPTY_PORT_Z		= 44;
const int AX_EMPTY_TRANS2_Y		= 45;
const int AX_PM_TRIGGER			= 46;
///////////////////////////////////////////////////////////////////////////////
// Motion Status

typedef struct tag_AXIS_STATUS {
	double dPos;		// Motor Current Position
	double dVel;		// Motor Current Velocity
	BOOL bSOn;			// Servo On Status
	BOOL bOrg;			// Origin Sensor
	BOOL bELP;			// End Limit Positive
	BOOL bELN;			// End Limit Negative
	BOOL bALM;			// Driver Alarm
	BOOL bInP;			// In Position
	BOOL bRun;			// Running
	BOOL bHom;			// Home Complete
} AXIS_STATUS;

///////////////////////////////////////////////////////////////////////////////
// Motion Parameter

typedef struct tag_AXIS_PARAM {
	double dSpeedM;	// Move Speed
	double dSpeedJ;	// Jog Speed
	double dAccel;	// Move Accel
} AXIS_PARAM;

///////////////////////////////////////////////////////////////////////////////
// DIO Count

const long DIO_MODULE_COUNT = 16;		// IO Module (PCI-DI64R 2EA, PCI-DO64R 2EA)

///////////////////////////////////////////////////////////////////////////////
// 변환 타입

typedef union tag_DXY_DATA {
	DWORD nValue;
	struct {
		DWORD b0000: 1;		// 0000
		DWORD b0001: 1;		// 0001
		DWORD b0002: 1;		// 0002
		DWORD b0003: 1;		// 0003
		DWORD b0004: 1;		// 0004
		DWORD b0005: 1;		// 0005
		DWORD b0006: 1;		// 0006
		DWORD b0007: 1;		// 0007
		DWORD b0008: 1;		// 0008
		DWORD b0009: 1;		// 0009
		DWORD b0010: 1;		// 0010
		DWORD b0011: 1;		// 0011
		DWORD b0012: 1;		// 0012
		DWORD b0013: 1;		// 0013
		DWORD b0014: 1;		// 0014
		DWORD b0015: 1;		// 0015
		DWORD b0016: 1;		// 0016
		DWORD b0017: 1;		// 0017
		DWORD b0018: 1;		// 0018
		DWORD b0019: 1;		// 0019
		DWORD b0020: 1;		// 0020
		DWORD b0021: 1;		// 0021
		DWORD b0022: 1;		// 0022
		DWORD b0023: 1;		// 0023
		DWORD b0024: 1;		// 0024
		DWORD b0025: 1;		// 0025
		DWORD b0026: 1;		// 0026
		DWORD b0027: 1;		// 0027
		DWORD b0028: 1;		// 0028
		DWORD b0029: 1;		// 0029
		DWORD b0030: 1;		// 0030
		DWORD b0031: 1;		// 0031
	};
} DXY_DATA;	// Digital Input Output

///////////////////////////////////////////////////////////////////////////////
#ifdef EDITION_2ND
// Input (X0000 - X1031)
typedef union tag_DX_DATA_00 {
	DWORD nValue;
	struct {

		DWORD i0001 : 1;					// X0000
		DWORD i0002 : 1;					// X0001
		DWORD iLoadPort1Bottom : 1;			// X0002
		DWORD i0003 : 1;					// X0003
		DWORD iLoadPort1SlideClose : 1;		// X0004
		DWORD iLoadPort1SlideOpen : 1;		// X0005
		DWORD iLoadPortAreaCheck : 1;		// X0006
		DWORD i0007 : 1;					// X0007
		DWORD iLoadPort1Support1In : 1;		// X0008
		DWORD iLoadPort1Support1Out : 1;	// X0009
		DWORD iLoadPort1Support2In : 1;		// X0010
		DWORD iLoadPort1Support2Out : 1;	// X0011
		DWORD iLoadPort1SlideLock : 1;		// X0012
		DWORD iLoadPort1SlideUnlock : 1;	// X0013
		DWORD i0014 : 1;					// X0014
		DWORD i0015 : 1;					// X0015
		DWORD i0016 : 1;					// X0016
		DWORD i0017 : 1;					// X0017
		DWORD iLoadPort2Bottom : 1;			// X0018
		DWORD i0019 : 1;					// X0019
		DWORD iLoadPort2SlideClose : 1;		// X0020
		DWORD iLoadPort2SlideOpen : 1;		// X0021
		DWORD i0022 : 1;					// X0022
		DWORD i0023 : 1;					// X0023
		DWORD iLoadPort2Support1In : 1;		// X0024
		DWORD iLoadPort2Support1Out : 1;	// X0025
		DWORD iLoadPort2Support2In : 1;		// X0026
		DWORD iLoadPort2Support2Out : 1;	// X0027
		DWORD iLoadPort2SlideLock : 1;		// X0028
		DWORD iLoadPort2SlideUnlock : 1;	// X0029
		DWORD i0030 : 1;					// X0030
		DWORD i0031 : 1;					// X0031
	};
} DX_DATA_00;	// Digital Input 00

typedef union tag_DX_DATA_01 {
	DWORD nValue;
	struct {
		DWORD i0100 : 1;					// X0100
		DWORD iLoadStageExist : 1;			// X0101
		DWORD i0102 : 1;					// X0102
		DWORD i0103 : 1;					// X0103
		DWORD iLoadStageMasterIn : 1;		// X0104
		DWORD iLoadStageMasterOut : 1;		// X0105
		DWORD iLoadStageSlaveIn : 1;		// X0106
		DWORD iLoadStageSlaveOut : 1;		// X0107
		DWORD iEmptyPortTopCheck : 1;		// X0108
		DWORD i0109 : 1;					// X0109
		DWORD iEmptyPortSlideClose : 1;		// X0110
		DWORD iEmptyPortSlideOpen : 1;		// X0111
		DWORD iEmptyPortAreaCheck : 1;		// X0112
		DWORD i0113 : 1;					// X0113
		DWORD iEmptyPortExist : 1;			// X0114
		DWORD i0115 : 1;					// X0115
		DWORD i0116 : 1;					// X0116
		DWORD iLoadPickerExist : 1;			// X0117
		DWORD i0118 : 1;					// X0118
		DWORD i0119 : 1;					// X0119
		DWORD i0120 : 1;					// X0120
		DWORD i0121 : 1;					// X0121
		DWORD i0122 : 1;					// X0122
		DWORD i0123 : 1;					// X0123
		DWORD iLoadPickerUp : 1;			// X0124
		DWORD iLoadPickerDown : 1;			// X0125
		DWORD iLoadPickerMasterIn : 1;		// X0126
		DWORD iLoadPickerMasterOut : 1;		// X0127
		DWORD iLoadPickerSlaveIn : 1;		// X0128
		DWORD iLoadPickerSlaveOut : 1;		// X0129
		DWORD i0130 : 1;					// X0130
		DWORD i0131 : 1;					// X0131
	};
} DX_DATA_01;	// Digital Input 01

typedef union tag_DX_DATA_02 {
	DWORD nValue;
	struct {
		DWORD iAnglePortExist : 1;			// X0200
		DWORD i0201 : 1;					// X0201
		DWORD iAnglePortSupport1In : 1;		// X0202
		DWORD iAnglePortSupport1Out : 1;	// X0203
		DWORD iAnglePortSupport2In : 1;		// X0204
		DWORD iAnglePortSupport2Out : 1;	// X0205
		DWORD iAnglePortMasterIn : 1;		// X0206
		DWORD iAnglePortMasterOut : 1;		// X0207
		DWORD iAnglePortSlaveIn : 1;		// X0208
		DWORD iAnglePortSlaveOut : 1;		// X0209
		DWORD i0210 : 1;					// X0210
		DWORD i0211 : 1;					// X0211
		DWORD i0212 : 1;					// X0212
		DWORD i0213 : 1;					// X0213
		DWORD i0214 : 1;					// X0214
		DWORD i0215 : 1;					// X0215
		DWORD i0216	: 1;					// X0216 //iAngleStage1Vacuum : 1;
		DWORD i0217 : 1;					// X0217
		DWORD iAngleStage1Normal : 1;		// X0218
		DWORD iAngleStage1Rotate : 1;		// X0219
		DWORD iAngleStage1AlignOut : 1;		// X0220
		DWORD iAngleStage1AlignIn : 1;		// X0221
		DWORD iAngleStage1Exist : 1;		// X0222
		DWORD i0223 : 1;					// X0223
		DWORD i0224 : 1;					// X0224 //iAngleStage2Vacuum : 1;
		DWORD i0225 : 1;					// X0225
		DWORD iAngleStage2Normal : 1;		// X0226
		DWORD iAngleStage2Rotate : 1;		// X0227
		DWORD iAngleStage2AlignOut : 1;		// X0228
		DWORD iAngleStage2AlignIn : 1;		// X0229
		DWORD iAngleStage2Exist : 1;		// X0230
		DWORD i0231 : 1;					// X0231
	};
} DX_DATA_02;	// Digital Input 02

typedef union tag_DX_DATA_03 {
	DWORD nValue;
	struct {
		DWORD iBtm1PickerOpen01 : 1;		// X0300
		DWORD iBtm1PickerExist01 : 1;		// X0301
		DWORD iBtm1PickerOpen02 : 1;		// X0302
		DWORD iBtm1PickerExist02 : 1;		// X0303
		DWORD iBtm1PickerOpen03 : 1;		// X0304
		DWORD iBtm1PickerExist03 : 1;		// X0305
		DWORD iBtm1PickerOpen04 : 1;		// X0306
		DWORD iBtm1PickerExist04 : 1;		// X0307
		DWORD i0308 : 1;					// X0308
		DWORD i0309 : 1;					// X0309
		DWORD iBtm1PickerOpen05 : 1;		// X0310
		DWORD iBtm1PickerExist05 : 1;		// X0311
		DWORD iBtm1PickerOpen06 : 1;		// X0312
		DWORD iBtm1PickerExist06 : 1;		// X0313
		DWORD iBtm1PickerOpen07 : 1;		// X0314
		DWORD iBtm1PickerExist07 : 1;		// X0315
		DWORD iBtm1PickerOpen08 : 1;		// X0316
		DWORD iBtm1PickerExist08 : 1;		// X0317
		DWORD i0318 : 1;					// X0318
		DWORD i0319 : 1;					// X0319
		DWORD i0320 : 1;					// X0320
		DWORD i0321 : 1;					// X0321
		DWORD iBtm1PickerUp01 : 1;			// X0322
		DWORD iBtm1PickerDown01 : 1;		// X0323
		DWORD iBtm1PickerUp02 : 1;			// X0324
		DWORD iBtm1PickerDown02 : 1;		// X0325
		DWORD iBtm1PickerUp03 : 1;			// X0326
		DWORD iBtm1PickerDown03 : 1;		// X0327
		DWORD iBtm1PickerUp04 : 1;			// X0328
		DWORD iBtm1PickerDown04 : 1;		// X0329
		DWORD i0330 : 1;					// X0330
		DWORD i0331 : 1;					// X0331
	};
} DX_DATA_03;	// Digital Input 03

typedef union tag_DX_DATA_04 {
	DWORD nValue;
	struct {
		DWORD iBtm1PickerVac01 : 1;			// X0400
		DWORD i0401 : 1;					// X0401
		DWORD iBtm1PickerVac02 : 1;			// X0402
		DWORD i0403 : 1;					// X0403
		DWORD iBtm1PickerVac03 : 1;			// X0404
		DWORD i0405 : 1;					// X0405
		DWORD iBtm1PickerVac04 : 1;			// X0406
		DWORD i0407 : 1;					// X0407
		DWORD i0408 : 1;					// X0408
		DWORD i0409 : 1;					// X0409
		DWORD iBtm1PickerVac05 : 1;			// X0410
		DWORD i0411 : 1;					// X0411
		DWORD iBtm1PickerVac06 : 1;			// X0412
		DWORD i0413 : 1;					// X0413
		DWORD iBtm1PickerVac07 : 1;			// X0414
		DWORD i0415 : 1;					// X0415
		DWORD iBtm1PickerVac08 : 1;			// X0416
		DWORD i0417 : 1;					// X0417
		DWORD i0418 : 1;					// X0418
		DWORD i0419 : 1;					// X0419
		DWORD i0420 : 1;					// X0420
		DWORD i0421 : 1;					// X0421
		DWORD iBtm1PickerUp05 : 1;			// X0422
		DWORD iBtm1PickerDown05 : 1;		// X0423
		DWORD iBtm1PickerUp06 : 1;			// X0424
		DWORD iBtm1PickerDown06 : 1;		// X0425
		DWORD iBtm1PickerUp07 : 1;			// X0426
		DWORD iBtm1PickerDown07 : 1;		// X0427
		DWORD iBtm1PickerUp08 : 1;			// X0428
		DWORD iBtm1PickerDown08 : 1;		// X0429
		DWORD i0430 : 1;					// X0430
		DWORD i0431 : 1;					// X0431
	};
} DX_DATA_04;	// Digital Input 04

typedef union tag_DX_DATA_05 {
	DWORD nValue;
	struct {
		DWORD iInspectStage1Up : 1;			// X0500
		DWORD iInspectStage1Down : 1;		// X0501
		DWORD i0502 : 1;					// X0502
		DWORD i0503 : 1;					// X0503
		DWORD iInspectStage2Up : 1;			// X0504
		DWORD iInspectStage2Down : 1;		// X0505
		DWORD iInspectStage2Fwd : 1;		// X0506
		DWORD iInspectStage2Bwd : 1;		// X0507
		DWORD iInspectStage3Up : 1;			// X0508
		DWORD iInspectStage3Down : 1;		// X0509
		DWORD iInspectStage3Fwd : 1;		// X0510
		DWORD iInspectStage3Bwd : 1;		// X0511
		DWORD i0512 : 1;					// X0512
		DWORD i0513 : 1;					// X0513
		DWORD i0514 : 1;					// X0514
		DWORD i0515 : 1;					// X0515
		DWORD iInspectStage1Vac01 : 1;		// X0516
		DWORD iInspectStage1Vac02 : 1;		// X0517
		DWORD iInspectStage1Vac03 : 1;		// X0518
		DWORD iInspectStage1Vac04 : 1;		// X0519
		DWORD i0520 : 1;					// X0520
		DWORD iInspectStage1Vac05 : 1;		// X0521
		DWORD iInspectStage1Vac06 : 1;		// X0522
		DWORD iInspectStage1Vac07 : 1;		// X0523
		DWORD iInspectStage1Vac08 : 1;		// X0524
		DWORD i0525 : 1;					// X0525
		DWORD i0526 : 1;					// X0526
		DWORD i0527 : 1;					// X0527
		DWORD i0528 : 1;					// X0528
		DWORD i0529 : 1;					// X0529
		DWORD i0530 : 1;					// X0530
		DWORD i0531 : 1;					// X0531
	};
} DX_DATA_05;	// Digital Input 05

typedef union tag_DX_DATA_06 {
	DWORD nValue;
	struct {
		DWORD iInspectStage2Vac01 : 1;		// X0600
		DWORD iInspectStage2Vac02 : 1;		// X0601
		DWORD iInspectStage2Vac03 : 1;		// X0602
		DWORD iInspectStage2Vac04 : 1;		// X0603
		DWORD i0604 : 1;					// X0604
		DWORD iInspectStage2Vac05 : 1;		// X0605
		DWORD iInspectStage2Vac06 : 1;		// X0606
		DWORD iInspectStage2Vac07 : 1;		// X0607
		DWORD iInspectStage2Vac08 : 1;		// X0608
		DWORD i0609 : 1;					// X0609
		DWORD i0610 : 1;					// X0610
		DWORD i0611 : 1;					// X0611
		DWORD iCmAlignSlaveClose  : 1;		// X0612
		DWORD iCmAlignSlaveOpen	  : 1;		// X0613
		DWORD i0614 : 1;					// X0614
		DWORD i0615 : 1;					// X0615
		DWORD iInspectStage3Vac01 : 1;		// X0616
		DWORD iInspectStage3Vac02 : 1;		// X0617
		DWORD iInspectStage3Vac03 : 1;		// X0618
		DWORD iInspectStage3Vac04 : 1;		// X0619
		DWORD i0620 : 1;					// X0620
		DWORD iInspectStage3Vac05 : 1;		// X0621
		DWORD iInspectStage3Vac06 : 1;		// X0622
		DWORD iInspectStage3Vac07 : 1;		// X0623
		DWORD iInspectStage3Vac08 : 1;		// X0624
		DWORD i0625 : 1;					// X0625
		DWORD iCmAlignMasterClose : 1;		// X0626
		DWORD iCmAlignMasterOpen  : 1;		// X0627
		DWORD iTop1Mirror1Up : 1;			// X0628
		DWORD iTop1Mirror1Down : 1;			// X0629
		DWORD iTop1Mirror2Up : 1;			// X0630
		DWORD iTop1Mirror2Down : 1;			// X0631
	};
} DX_DATA_06;	// Digital Input 06

typedef union tag_DX_DATA_07 {
	DWORD nValue;
	struct {
		DWORD iBtm2PickerOpen01 : 1; 		// X0700
		DWORD iBtm2PickerExist01 : 1;		// X0701
		DWORD iBtm2PickerOpen02 : 1; 		// X0702
		DWORD iBtm2PickerExist02 : 1;		// X0703
		DWORD iBtm2PickerOpen03 : 1; 		// X0704
		DWORD iBtm2PickerExist03 : 1;		// X0705
		DWORD iBtm2PickerOpen04 : 1; 		// X0706
		DWORD iBtm2PickerExist04 : 1;		// X0707
		DWORD i0708 : 1;			 		// X0708
		DWORD i0709 : 1;			 		// X0709
		DWORD iBtm2PickerOpen05 : 1; 		// X0710
		DWORD iBtm2PickerExist05 : 1;		// X0711
		DWORD iBtm2PickerOpen06 : 1; 		// X0712
		DWORD iBtm2PickerExist06 : 1;		// X0713
		DWORD iBtm2PickerOpen07 : 1; 		// X0714
		DWORD iBtm2PickerExist07 : 1;		// X0715
		DWORD iBtm2PickerOpen08 : 1; 		// X0716
		DWORD iBtm2PickerExist08 : 1;		// X0717
		DWORD i0718 : 1;					// X0718
		DWORD i0719 : 1;					// X0719
		DWORD i0720 : 1;					// X0720
		DWORD i0721 : 1;					// X0721
		DWORD iBtm2PickerUp01 : 1;			// X0722
		DWORD iBtm2PickerDown01 : 1;		// X0723
		DWORD iBtm2PickerUp02 : 1;			// X0724
		DWORD iBtm2PickerDown02 : 1;		// X0725
		DWORD iBtm2PickerUp03 : 1;			// X0726
		DWORD iBtm2PickerDown03 : 1;		// X0727
		DWORD iBtm2PickerUp04 : 1;			// X0728
		DWORD iBtm2PickerDown04 : 1;		// X0729
		DWORD i0730 : 1;					// X0730
		DWORD i0731 : 1;					// X0731
	};
} DX_DATA_07;	// Digital Input 07

typedef union tag_DX_DATA_08 {
	DWORD nValue;
	struct {
		DWORD iBtm2PickerVac01 : 1;			// X0800
		DWORD i0801 : 1;					// X0801
		DWORD iBtm2PickerVac02 : 1;			// X0802
		DWORD i0803 : 1;					// X0803
		DWORD iBtm2PickerVac03 : 1;			// X0804
		DWORD i0805 : 1;					// X0805
		DWORD iBtm2PickerVac04 : 1;			// X0806
		DWORD i0807 : 1;					// X0807
		DWORD i0808 : 1;					// X0808
		DWORD i0809 : 1;					// X0809
		DWORD iBtm2PickerVac05 : 1;			// X0810
		DWORD i0811 : 1;					// X0811
		DWORD iBtm2PickerVac06 : 1;			// X0812
		DWORD i0813 : 1;					// X0813
		DWORD iBtm2PickerVac07 : 1;			// X0814
		DWORD i0815 : 1;					// X0815
		DWORD iBtm2PickerVac08 : 1;			// X0816
		DWORD i0817 : 1;					// X0817
		DWORD i0818 : 1;					// X0818
		DWORD i0819 : 1;					// X0819
		DWORD i0820 : 1;					// X0820
		DWORD i0821 : 1;					// X0821
		DWORD iBtm2PickerUp05 : 1;			// X0822
		DWORD iBtm2PickerDown05 : 1;		// X0823
		DWORD iBtm2PickerUp06 : 1;			// X0824
		DWORD iBtm2PickerDown06 : 1;		// X0825
		DWORD iBtm2PickerUp07 : 1;			// X0826
		DWORD iBtm2PickerDown07 : 1;		// X0827
		DWORD iBtm2PickerUp08 : 1;			// X0828
		DWORD iBtm2PickerDown08 : 1;		// X0829
		DWORD i0830 : 1;					// X0830
		DWORD i0831 : 1;					// X0831
	};
} DX_DATA_08;	// Digital Input 08

typedef union tag_DX_DATA_09 {
	DWORD nValue;
	struct {
		DWORD iBufferStage1Vacuum : 1;		// X0900
		DWORD i0901 : 1;					// X0901
		DWORD iBufferStage1Normal : 1;		// X0902
		DWORD iBufferStage1Rotate : 1;		// X0903
		DWORD iBufferStage1Up : 1;			// X0904
		DWORD iBufferStage1Down : 1;		// X0905
		DWORD i0906 : 1;					// X0906
		DWORD i0907 : 1;					// X0907
		DWORD iBufferStage2Vacuum : 1;		// X0908
		DWORD i0909 : 1;					// X0909
		DWORD iBufferStage2Normal : 1;		// X0910
		DWORD iBufferStage2Rotate : 1;		// X0911
		DWORD iBufferStage2Up : 1;			// X0912
		DWORD iBufferStage2Down : 1;		// X0913
		DWORD i0914 : 1;					// X0914
		DWORD i0915 : 1;					// X0915
		DWORD iNgStageExist1 : 1;			// X0916
		DWORD iNgStageExist2 : 1;			// X0917
		DWORD iNgStageExist3 : 1;			// X0918
		DWORD iNgStageExist4 : 1;			// X0919
		DWORD iNgStageGuideClose : 1;		// X0920
		DWORD i0921 : 1;					// X0921
		DWORD i0922 : 1;					// X0922
		DWORD i0923 : 1;					// X0923
		DWORD iNgPortSlideClose : 1;		// X0924
		DWORD iNgPortSlideOpen : 1;			// X0925
		DWORD iNgPortSlideLock : 1;			// X0926
		DWORD iNgPortSlideUnlock : 1;		// X0927
		DWORD i0928 : 1;					// X0928
		DWORD i0929 : 1;					// X0929
		DWORD iNgPortAreaCheck : 1;			// X0930
		DWORD i0931 : 1;					// X0931
	};
} DX_DATA_09;	// Digital Input 09

typedef union tag_DX_DATA_10 {
	DWORD nValue;
	struct {
		DWORD iSortPicker1Up1 : 1;			// X1000
		DWORD iSortPicker1Down1 : 1;		// X1001
		DWORD iSortPicker1Up2 : 1;			// X1002
		DWORD iSortPicker1Down2 : 1;		// X1003
		DWORD iSortPicker1Up3 : 1;			// X1004
		DWORD iSortPicker1Down3 : 1;		// X1005
		DWORD iSortPicker1Up4 : 1;			// X1006
		DWORD iSortPicker1Down4 : 1;		// X1007
		DWORD i1008 : 1;					// X1008
		DWORD i1009 : 1;					// X1009
		DWORD i1010 : 1;					// X1010
		DWORD i1011 : 1;					// X1011
		DWORD i1012 : 1;					// X1012
		DWORD i1013 : 1;					// X1013
		DWORD i1014 : 1;					// X1014
		DWORD i1015 : 1;					// X1015
		DWORD iSortPicker1Open1 : 1;		// X1016
		DWORD iSortPicker1Exist1 : 1;		// X1017
		DWORD iSortPicker1Open2 : 1;		// X1018
		DWORD iSortPicker1Exist2 : 1;		// X1019
		DWORD iSortPicker1Open3 : 1;		// X1020
		DWORD iSortPicker1Exist3 : 1;		// X1021
		DWORD iSortPicker1Open4 : 1;		// X1022
		DWORD iSortPicker1Exist4 : 1;		// X1023
		DWORD i1024 : 1;					// X1024
		DWORD i1025 : 1;					// X1025
		DWORD i1026 : 1;					// X1026
		DWORD iNgBufferVac01 : 1;			// X1027
		DWORD iNgBufferVac02 : 1;			// X1028
		DWORD iNgBufferVac03 : 1;			// X1029
		DWORD iNgBufferVac04 : 1;			// X1030
		DWORD i1031 : 1;					// X1031
	};
} DX_DATA_10;	// Digital Input 10

typedef union tag_DX_DATA_11 {
	DWORD nValue;
	struct {
		DWORD iSortPicker2Up1 : 1;			// X1100
		DWORD iSortPicker2Down1 : 1;		// X1101
		DWORD iSortPicker2Up2 : 1;			// X1102
		DWORD iSortPicker2Down2 : 1;		// X1103
		DWORD iSortPicker2Up3 : 1;			// X1104
		DWORD iSortPicker2Down3 : 1;		// X1105
		DWORD iSortPicker2Up4 : 1;			// X1106
		DWORD iSortPicker2Down4 : 1;		// X1107
		DWORD i1108 : 1;					// X1108
		DWORD i1109 : 1;					// X1109
		DWORD i1110 : 1;					// X1110
		DWORD i1111 : 1;					// X1111
		DWORD i1112 : 1;					// X1112
		DWORD i1113 : 1;					// X1113
		DWORD i1114 : 1;					// X1114
		DWORD i1115 : 1;					// X1115
		DWORD iSortPicker2Open1 : 1;		// X1116
		DWORD iSortPicker2Exist1 : 1;		// X1117
		DWORD iSortPicker2Open2 : 1;		// X1118
		DWORD iSortPicker2Exist2 : 1;		// X1119
		DWORD iSortPicker2Open3 : 1;		// X1120
		DWORD iSortPicker2Exist3 : 1;		// X1121
		DWORD iSortPicker2Open4 : 1;		// X1122
		DWORD iSortPicker2Exist4 : 1;		// X1123
		DWORD i1124 : 1;					// X1124
		DWORD i1125 : 1;					// X1125
		DWORD i1126 : 1;					// X1126
		DWORD iNgBufferVac05 : 1;			// X1127
		DWORD iNgBufferVac06 : 1;			// X1128
		DWORD iNgBufferVac07 : 1;			// X1129
		DWORD iNgBufferVac08 : 1;			// X1130
		DWORD i1131 : 1;					// X1131
	};
} DX_DATA_11;	// Digital Input 11

typedef union tag_DX_DATA_12 {
	DWORD nValue;
	struct {
		DWORD iGoodPortUpper : 1;			// X1200
		DWORD iGoodPortLower : 1;			// X1201
		DWORD iGoodPortBottom : 1;			// X1202
		DWORD i1203 : 1;					// X1203
		DWORD iGoodPortSlideClose : 1;		// X1204
		DWORD iGoodPortSlideOpen : 1;		// X1205
		DWORD iGoodPortAreaCheck : 1;		// X1206
		DWORD i1207 : 1;					// X1207
		DWORD iGoodPortSlideLock : 1;		// X1208
		DWORD iGoodPortSlideUnlock : 1;		// X1209
		DWORD iGoodTrayBufferBottom : 1;	// X1210
		DWORD i1211 : 1;					// X1211
		DWORD iGoodTrayBuffSupport1In : 1;	// X1212
		DWORD iGoodTrayBuffSupport1Out : 1;	// X1213
		DWORD iGoodTrayBuffSupport2In : 1;	// X1214
		DWORD iGoodTrayBuffSupport2Out : 1;	// X1215
		DWORD i1216 : 1;					// X1216
		DWORD iGoodStage1Exist : 1;			// X1217
		DWORD i1218 : 1;					// X1218
		DWORD i1219 : 1;					// X1219
		DWORD iGoodStage1MasterIn : 1;		// X1220
		DWORD iGoodStage1MasterOut : 1;		// X1221
		DWORD iGoodStage1SlaveIn : 1;		// X1222
		DWORD iGoodStage1SlaveOut : 1;		// X1223
		DWORD i1224 : 1;					// X1224
		DWORD iGoodStage2Exist : 1;			// X1225
		DWORD i1226 : 1;					// X1226
		DWORD i1227 : 1;					// X1227
		DWORD iGoodStage2MasterIn : 1;		// X1228
		DWORD iGoodStage2MasterOut : 1;		// X1229
		DWORD iGoodStage2SlaveIn : 1;		// X1230
		DWORD iGoodStage2SlaveOut : 1;		// X1231
	};
} DX_DATA_12;	// Digital Input 12

typedef union tag_DX_DATA_13 {
	DWORD nValue;
	struct {
		DWORD i1300 : 1;					// X1300
		DWORD iEmptyTrans1Exist : 1;		// X1301
		DWORD i1302 : 1;					// X1302
		DWORD i1303 : 1;					// X1303
		DWORD i1304 : 1;					// X1304
		DWORD i1305 : 1;					// X1305
		DWORD i1306 : 1;					// X1306
		DWORD i1307 : 1;					// X1307
		DWORD iEmptyTrans1Up : 1;			// X1308
		DWORD iEmptyTrans1Down : 1;			// X1309
		DWORD iEmptyTrans1MasterIn : 1;		// X1310
		DWORD iEmptyTrans1MasterOut : 1;	// X1311
		DWORD iEmptyTrans1SlaveIn : 1;		// X1312
		DWORD iEmptyTrans1SlaveOut : 1;		// X1313
		DWORD i1314 : 1;					// X1314
		DWORD i1315 : 1;					// X1315
		DWORD iEmptyTrans2Exist : 1;		// X1316
		DWORD i1317 : 1;					// X1317
		DWORD i1318 : 1;					// X1318
		DWORD i1319 : 1;					// X1319
		DWORD i1320 : 1;					// X1320
		DWORD i1321 : 1;					// X1321
		DWORD i1322 : 1;					// X1322
		DWORD i1323 : 1;					// X1323
		DWORD iEmptyTrans2Up : 1;			// X1324
		DWORD iEmptyTrans2Down : 1;			// X1325
		DWORD iEmptyTrans2MasterIn : 1;		// X1326
		DWORD iEmptyTrans2MasterOut : 1;	// X1327
		DWORD iEmptyTrans2SlaveIn : 1;		// X1328
		DWORD iEmptyTrans2SlaveOut : 1;		// X1329
		DWORD i1330 : 1;					// X1330
		DWORD i1331 : 1;					// X1331
	};
} DX_DATA_13;	// Digital Input 13

typedef union tag_DX_DATA_14 {
	DWORD nValue;
	struct {
		DWORD iEmgSw1 : 1;					// X1400
		DWORD iEmgSw2 : 1;					// X1401
		DWORD iEmgSw3 : 1;					// X1402
		DWORD iEmgSw4 : 1;					// X1403
		DWORD iEmgSw5 : 1;					// X1404
		DWORD iEmgSw6 : 1;					// X1405
		DWORD iMainAir1 : 1;				// X1406
		DWORD iMainAir2 : 1;				// X1407
		DWORD iMainAir3 : 1;				// X1408
		DWORD i1409 : 1;					// X1409
		DWORD i1410 : 1;					// X1410
		DWORD i1411 : 1;					// X1411
		DWORD iStartSw : 1;					// X1412
		DWORD i1413 : 1;					// X1413
		DWORD i1414 : 1;					// X1414
		DWORD iStopSw : 1;					// X1415
		DWORD i1416 : 1;					// X1416
		DWORD i1417 : 1;					// X1417
		DWORD iResetSw : 1;					// X1418
		DWORD i1419 : 1;					// X1419
		DWORD i1420 : 1;					// X1420
		DWORD i1421 : 1;					// X1421
		DWORD i1422 : 1;					// X1422
		DWORD i1423 : 1;					// X1423
		DWORD iLoad1Sw : 1;					// X1424
		DWORD iLoad2Sw : 1;					// X1425
		DWORD iNgSw : 1;					// X1426
		DWORD iGoodSw : 1;					// X1427
		DWORD iEmptySw : 1;					// X1428
		DWORD i1429 : 1;					// X1429
		DWORD i1430 : 1;					// X1430
		DWORD i1431 : 1;					// X1431
	};
} DX_DATA_14;	// Digital Input 14

typedef union tag_DX_DATA_15 {
	DWORD nValue;
	struct {
		DWORD iDoor01Unlock : 1;			// X1500
		DWORD iDoor02Unlock : 1;			// X1501
		DWORD iDoor03Unlock : 1;			// X1502
		DWORD iDoor04Unlock : 1;			// X1503
		DWORD iDoor05Unlock : 1;			// X1504
		DWORD iDoor06Unlock : 1;			// X1505
		DWORD iDoor07Unlock : 1;			// X1506
		DWORD iDoor08Unlock : 1;			// X1507
		DWORD iDoor09Unlock : 1;			// X1508
		DWORD iDoor10Unlock : 1;			// X1509
		DWORD iDoor11Unlock : 1;			// X1510
		DWORD iDoor12Unlock : 1;			// X1511
		DWORD iDoor13Unlock : 1;			// X1512
		DWORD iDoor14Unlock : 1;			// X1513
		DWORD iDoor15Unlock : 1;			// X1514
		DWORD iDoor16Unlock : 1;			// X1515
		DWORD iDoor17Unlock : 1;			// X1516
		DWORD iDoor18Unlock : 1;			// X1517
		DWORD iDoor19Unlock : 1;			// X1518
		DWORD iDoor20Unlock : 1;			// X1519
		DWORD i1520 : 1;					// X1520
		DWORD i1521 : 1;					// X1521
		DWORD i1522 : 1;					// X1522
		DWORD i1523 : 1;					// X1523
		DWORD i1524 : 1;					// X1524
		DWORD i1525 : 1;					// X1525
		DWORD i1526 : 1;					// X1526
		DWORD i1527 : 1;					// X1527
		DWORD i1528 : 1;					// X1528
		DWORD i1529 : 1;					// X1529
		DWORD i1530 : 1;					// X1530
		DWORD i1531 : 1;					// X1531
	};
} DX_DATA_15;	// Digital Input 15

///////////////////////////////////////////////////////////////////////////////
// Output (Y0000 - Y1331)

typedef union tag_DY_DATA_00 {
	DWORD nValue;
	struct {
		DWORD o0000 : 1;					// Y0000
		DWORD o0001 : 1;					// Y0001
		DWORD o0002 : 1;					// Y0002
		DWORD o0003 : 1;					// Y0003
		DWORD o0004 : 1;					// Y0004
		DWORD o0005 : 1;					// Y0005
		DWORD o0006 : 1;					// Y0006
		DWORD o0007 : 1;					// Y0007
		DWORD oLoadPort1SupportIn : 1;		// Y0008
		DWORD oLoadPort1SupportOut : 1;		// Y0009
		DWORD o0010 : 1;					// Y0010
		DWORD o0011 : 1;					// Y0011
		DWORD oLoadPort1SlideLock : 1;		// Y0012
		DWORD oLoadPort1SlideUnlock : 1;	// Y0013
		DWORD o0014 : 1;					// Y0014
		DWORD o0015 : 1;					// Y0015
		DWORD o0016 : 1;					// Y0016
		DWORD o0017 : 1;					// Y0017
		DWORD o0018 : 1;					// Y0018
		DWORD o0019 : 1;					// Y0019
		DWORD o0020 : 1;					// Y0020
		DWORD o0021 : 1;					// Y0021
		DWORD o0022 : 1;					// Y0022
		DWORD o0023 : 1;					// Y0023
		DWORD oLoadPort2SupportIn : 1;		// Y0024
		DWORD oLoadPort2SupportOut : 1;		// Y0025
		DWORD o0026 : 1;					// Y0026
		DWORD o0027 : 1;					// Y0027
		DWORD oLoadPort2SlideLock : 1;		// Y0028
		DWORD oLoadPort2SlideUnlock : 1;	// Y0029
		DWORD o0030 : 1;					// Y0030
		DWORD o0031 : 1;					// Y0031
	};
} DY_DATA_00;		// Digital Output 00

typedef union tag_DY_DATA_01 {
	DWORD nValue;
	struct {
		DWORD o0100 : 1;					// Y0100
		DWORD o0101 : 1;					// Y0101
		DWORD o0102 : 1;					// Y0102
		DWORD o0103 : 1;					// Y0103
		DWORD oLoadStageMasterIn : 1;		// Y0104
		DWORD o0105 : 1;					// Y0105
		DWORD oLoadStageSlaveIn : 1;		// Y0106
		DWORD o0107 : 1;					// Y0107
		DWORD o0108 : 1;					// Y0108
		DWORD o0109 : 1;					// Y0109
		DWORD o0110 : 1;					// Y0110
		DWORD o0111 : 1;					// Y0111
		DWORD o0112 : 1;					// Y0112
		DWORD o0113 : 1;					// Y0113
		DWORD o0114 : 1;					// Y0114
		DWORD o0115 : 1;					// Y0115
		DWORD o0116 : 1;					// Y0116
		DWORD o0117 : 1;					// Y0117
		DWORD o0118 : 1;					// Y0118
		DWORD o0119 : 1;					// Y0119
		DWORD o0120 : 1;					// Y0120
		DWORD o0121 : 1;					// Y0121
		DWORD o0122 : 1;					// Y0122
		DWORD o0123 : 1;					// Y0123
		DWORD oLoadPickerUp : 1;			// Y0124
		DWORD oLoadPickerDown : 1;			// Y0125
		DWORD oLoadPickerMasterIn : 1;		// Y0126
		DWORD oLoadPickerMasterOut : 1;		// Y0127
		DWORD oLoadPickerSlaveIn : 1;		// Y0128
		DWORD oLoadPickerSlaveOut : 1;		// Y0129
		DWORD o0130 : 1;					// Y0130
		DWORD o0131 : 1;					// Y0131
	};
} DY_DATA_01;		// Digital Output 01

typedef union tag_DY_DATA_02 {
	DWORD nValue;
	struct {
		DWORD o0200 : 1;					// Y0200
		DWORD o0201 : 1;					// Y0201
		DWORD oAnglePortSupportIn : 1;		// Y0202
		DWORD oAnglePortSupportOut : 1;		// Y0203
		DWORD o0204 : 1;					// Y0204
		DWORD o0205 : 1;					// Y0205
		DWORD oAnglePortMasterIn : 1;		// Y0206
		DWORD oAnglePortMasterOut : 1;		// Y0207
		DWORD oAnglePortSlaveIn : 1;		// Y0208
		DWORD oAnglePortSlaveOut : 1;		// Y0209
		DWORD o0210 : 1;					// Y0210
		DWORD o0211 : 1;					// Y0211
		DWORD o0212 : 1;					// Y0212
		DWORD o0213 : 1;					// Y0213
		DWORD o0214 : 1;					// Y0214
		DWORD o0215 : 1;					// Y0215
		DWORD oAngleStage1AlignIn : 1;		// Y0216
		DWORD o0217 : 1;					// Y0217
		DWORD o0218 : 1;					// Y0218
		DWORD oAngleStage1Rotate : 1;		// Y0219
		DWORD o0220 : 1;					// Y0220
		DWORD o0221 : 1;					// Y0221
		DWORD o0222 : 1;					// Y0222
		DWORD o0223 : 1;					// Y0223
		DWORD oAngleStage2AlignIn : 1;		// Y0224
		DWORD o0225 : 1;					// Y0225
		DWORD o0226 : 1;					// Y0226
		DWORD oAngleStage2Rotate : 1;		// Y0227
		DWORD o0228 : 1;					// Y0228
		DWORD o0229 : 1;					// Y0229
		DWORD o0230 : 1;					// Y0230
		DWORD o0231 : 1;					// Y0231
	};
} DY_DATA_02;		// Digital Output 02

typedef union tag_DY_DATA_03 {
	DWORD nValue;
	struct {
		DWORD oBtm1PickerOpen01 : 1;		// Y0300
		DWORD oBtm1PickerClose01 : 1;		// Y0301
		DWORD oBtm1PickerOpen02 : 1;		// Y0302
		DWORD oBtm1PickerClose02 : 1;		// Y0303
		DWORD oBtm1PickerOpen03 : 1;		// Y0304
		DWORD oBtm1PickerClose03 : 1;		// Y0305
		DWORD oBtm1PickerOpen04 : 1;		// Y0306
		DWORD oBtm1PickerClose04 : 1;		// Y0307
		DWORD o0308 : 1;					// Y0308
		DWORD o0309 : 1;					// Y0309
		DWORD oBtm1PickerOpen05 : 1;		// Y0310
		DWORD oBtm1PickerClose05 : 1;		// Y0311
		DWORD oBtm1PickerOpen06 : 1;		// Y0312
		DWORD oBtm1PickerClose06 : 1;		// Y0313
		DWORD oBtm1PickerOpen07 : 1;		// Y0314
		DWORD oBtm1PickerClose07 : 1;		// Y0315
		DWORD oBtm1PickerOpen08 : 1;		// Y0316
		DWORD oBtm1PickerClose08 : 1;		// Y0317
		DWORD o0318 : 1;					// Y0318
		DWORD o0319 : 1;					// Y0319
		DWORD o0320 : 1;					// Y0320
		DWORD o0321 : 1;					// Y0321
		DWORD o0322 : 1;					// Y0322
		DWORD oBtm1PickerDown01 : 1;		// Y0323
		DWORD o0324 : 1;					// Y0324
		DWORD oBtm1PickerDown02 : 1;		// Y0325
		DWORD o0326 : 1;					// Y0326
		DWORD oBtm1PickerDown03 : 1;		// Y0327
		DWORD o0328 : 1;					// Y0328
		DWORD oBtm1PickerDown04 : 1;		// Y0329
		DWORD o0330 : 1;					// Y0330
		DWORD o0331 : 1;					// Y0331
	};
} DY_DATA_03;		// Digital Output 03

typedef union tag_DY_DATA_04 {
	DWORD nValue;
	struct {
		DWORD oBtm1PickerVac01 : 1;			// Y0400
		DWORD oBtm1PickerAir01 : 1;			// Y0401
		DWORD oBtm1PickerVac02 : 1;			// Y0402
		DWORD oBtm1PickerAir02 : 1;			// Y0403
		DWORD oBtm1PickerVac03 : 1;			// Y0404
		DWORD oBtm1PickerAir03 : 1;			// Y0405
		DWORD oBtm1PickerVac04 : 1;			// Y0406
		DWORD oBtm1PickerAir04 : 1;			// Y0407
		DWORD o0408 : 1;					// Y0408
		DWORD o0409 : 1;					// Y0409
		DWORD oBtm1PickerVac05 : 1;			// Y0410
		DWORD oBtm1PickerAir05 : 1;			// Y0411
		DWORD oBtm1PickerVac06 : 1;			// Y0412
		DWORD oBtm1PickerAir06 : 1;			// Y0413
		DWORD oBtm1PickerVac07 : 1;			// Y0414
		DWORD oBtm1PickerAir07 : 1;			// Y0415
		DWORD oBtm1PickerVac08 : 1;			// Y0416
		DWORD oBtm1PickerAir08 : 1;			// Y0417
		DWORD o0418 : 1;					// Y0418
		DWORD o0419 : 1;					// Y0419
		DWORD o0420 : 1;					// Y0420
		DWORD o0421 : 1;					// Y0421
		DWORD o0422 : 1;					// Y0422
		DWORD oBtm1PickerDown05 : 1;		// Y0423
		DWORD o0424 : 1;					// Y0424
		DWORD oBtm1PickerDown06 : 1;		// Y0425
		DWORD o0426 : 1;					// Y0426
		DWORD oBtm1PickerDown07 : 1;		// Y0427
		DWORD o0428 : 1;					// Y0428
		DWORD oBtm1PickerDown08 : 1;		// Y0429
		DWORD o0430 : 1;					// Y0430
		DWORD o0431 : 1;					// Y0431
	};
} DY_DATA_04;		// Digital Output 04

typedef union tag_DY_DATA_05 {
	DWORD nValue;
	struct {
		DWORD oInspectStage1Up : 1;			// Y0500
		DWORD oInspectStage1Down : 1;		// Y0501
		DWORD o0502 : 1;					// Y0502
		DWORD o0503 : 1;					// Y0503
		DWORD oInspectStage2Up : 1;			// Y0504
		DWORD oInspectStage2Down : 1;		// Y0505
		DWORD oInspectStage2Fwd : 1;		// Y0506
		DWORD oInspectStage2Bwd : 1;		// Y0507
		DWORD oInspectStage3Up : 1;			// Y0508
		DWORD oInspectStage3Down : 1;		// Y0509
		DWORD oInspectStage3Fwd : 1;		// Y0510
		DWORD oInspectStage3Bwd : 1;		// Y0511
		DWORD o0512 : 1;					// Y0512
		DWORD o0513 : 1;					// Y0513
		DWORD o0514 : 1;					// Y0514
		DWORD o0515 : 1;					// Y0515
		DWORD oInspectStage1Vac01 : 1;		// Y0516
		DWORD oInspectStage1Vac02 : 1;		// Y0517
		DWORD oInspectStage1Vac03 : 1;		// Y0518
		DWORD oInspectStage1Vac04 : 1;		// Y0519
		DWORD o0520 : 1;					// Y0520
		DWORD oInspectStage1Vac05 : 1;		// Y0521
		DWORD oInspectStage1Vac06 : 1;		// Y0522
		DWORD oInspectStage1Vac07 : 1;		// Y0523
		DWORD oInspectStage1Vac08 : 1;		// Y0524
		DWORD o0525 : 1;					// Y0525
		DWORD o0526 : 1;					// Y0526
		DWORD o0527 : 1;					// Y0527
		DWORD o0528 : 1;					// Y0528
		DWORD o0529 : 1;					// Y0529
		DWORD o0530 : 1;					// Y0530
		DWORD o0531 : 1;					// Y0531
	};
} DY_DATA_05;		// Digital Output 05

typedef union tag_DY_DATA_06 {
	DWORD nValue;
	struct {
		DWORD oInspectStage2Vac01 : 1;		// Y0600
		DWORD oInspectStage2Vac02 : 1;		// Y0601
		DWORD oInspectStage2Vac03 : 1;		// Y0602
		DWORD oInspectStage2Vac04 : 1;		// Y0603
		DWORD o0604 : 1;					// Y0604
		DWORD oInspectStage2Vac05 : 1;		// Y0605
		DWORD oInspectStage2Vac06 : 1;		// Y0606
		DWORD oInspectStage2Vac07 : 1;		// Y0607
		DWORD oInspectStage2Vac08 : 1;		// Y0608
		DWORD o0609 : 1;					// Y0609
		DWORD o0610 : 1;					// Y0610
		DWORD o0611 : 1;					// Y0611
		DWORD oCmAlignSlaveClose  : 1;		// Y0612
		DWORD oCmAlignSlaveOpen   : 1;		// Y0613
		DWORD o0614 : 1;					// Y0614
		DWORD o0615 : 1;					// Y0615
		DWORD oInspectStage3Vac01 : 1;		// Y0616
		DWORD oInspectStage3Vac02 : 1;		// Y0617
		DWORD oInspectStage3Vac03 : 1;		// Y0618
		DWORD oInspectStage3Vac04 : 1;		// Y0619
		DWORD o0620 : 1;					// Y0620
		DWORD oInspectStage3Vac05 : 1;		// Y0621
		DWORD oInspectStage3Vac06 : 1;		// Y0622
		DWORD oInspectStage3Vac07 : 1;		// Y0623
		DWORD oInspectStage3Vac08 : 1;		// Y0624
		DWORD o0625 : 1;					// Y0625
		DWORD oCmAlignMasterClose : 1;		// Y0626
		DWORD oCmAlignMasterOpen  : 1;		// Y0627
		DWORD oTop1Mirror12Up	  : 1;		// Y0628
		DWORD oTop1Mirror12Down	  : 1;		// Y0629
		DWORD o0630 : 1;					// Y0630
		DWORD o0631 : 1;					// Y0631
	};
} DY_DATA_06;		// Digital Output 06

typedef union tag_DY_DATA_07 {
	DWORD nValue;
	struct {
		DWORD oBtm2PickerOpen01 : 1;		// Y0700
		DWORD oBtm2PickerClose01 : 1;		// Y0701
		DWORD oBtm2PickerOpen02 : 1;		// Y0702
		DWORD oBtm2PickerClose02 : 1;		// Y0703
		DWORD oBtm2PickerOpen03 : 1;		// Y0704
		DWORD oBtm2PickerClose03 : 1;		// Y0705
		DWORD oBtm2PickerOpen04 : 1;		// Y0706
		DWORD oBtm2PickerClose04 : 1;		// Y0707
		DWORD o0708 : 1;					// Y0708
		DWORD o0709 : 1;					// Y0709
		DWORD oBtm2PickerOpen05 : 1;		// Y0710
		DWORD oBtm2PickerClose05 : 1;		// Y0711
		DWORD oBtm2PickerOpen06 : 1;		// Y0712
		DWORD oBtm2PickerClose06 : 1;		// Y0713
		DWORD oBtm2PickerOpen07 : 1;		// Y0714
		DWORD oBtm2PickerClose07 : 1;		// Y0715
		DWORD oBtm2PickerOpen08 : 1;		// Y0716
		DWORD oBtm2PickerClose08 : 1;		// Y0717
		DWORD o0718 : 1;					// Y0718
		DWORD o0719 : 1;					// Y0719
		DWORD o0720 : 1;					// Y0720
		DWORD o0721 : 1;					// Y0721
		DWORD o0722 : 1;					// Y0722
		DWORD oBtm2PickerDown01 : 1;		// Y0723
		DWORD o0724 : 1;					// Y0724
		DWORD oBtm2PickerDown02 : 1;		// Y0725
		DWORD o0726 : 1;					// Y0726
		DWORD oBtm2PickerDown03 : 1;		// Y0727
		DWORD o0728 : 1;					// Y0728
		DWORD oBtm2PickerDown04 : 1;		// Y0729
		DWORD o0730 : 1;					// Y0730
		DWORD o0731 : 1;					// Y0731
	};
} DY_DATA_07;		// Digital Output 07

typedef union tag_DY_DATA_08 {
	DWORD nValue;
	struct {
		DWORD oBtm2PickerVac01 : 1;			// Y0800
		DWORD oBtm2PickerAir01 : 1;			// Y0801
		DWORD oBtm2PickerVac02 : 1;			// Y0802
		DWORD oBtm2PickerAir02 : 1;			// Y0803
		DWORD oBtm2PickerVac03 : 1;			// Y0804
		DWORD oBtm2PickerAir03 : 1;			// Y0805
		DWORD oBtm2PickerVac04 : 1;			// Y0806
		DWORD oBtm2PickerAir04 : 1;			// Y0807
		DWORD o0808 : 1;					// Y0808
		DWORD o0809 : 1;					// Y0809
		DWORD oBtm2PickerVac05 : 1;			// Y0810
		DWORD oBtm2PickerAir05 : 1;			// Y0811
		DWORD oBtm2PickerVac06 : 1;			// Y0812
		DWORD oBtm2PickerAir06 : 1;			// Y0813
		DWORD oBtm2PickerVac07 : 1;			// Y0814
		DWORD oBtm2PickerAir07 : 1;			// Y0815
		DWORD oBtm2PickerVac08 : 1;			// Y0816
		DWORD oBtm2PickerAir08 : 1;			// Y0817
		DWORD o0818 : 1;					// Y0818
		DWORD o0819 : 1;					// Y0819
		DWORD o0820 : 1;					// Y0820
		DWORD o0821 : 1;					// Y0821
		DWORD o0822 : 1;					// Y0822
		DWORD oBtm2PickerDown05 : 1;		// Y0823
		DWORD o0824 : 1;					// Y0824
		DWORD oBtm2PickerDown06 : 1;		// Y0825
		DWORD o0826 : 1;					// Y0826
		DWORD oBtm2PickerDown07 : 1;		// Y0827
		DWORD o0828 : 1;					// Y0828
		DWORD oBtm2PickerDown08 : 1;		// Y0829
		DWORD o0830 : 1;					// Y0830
		DWORD o0831 : 1;					// Y0831
	};
} DY_DATA_08;		// Digital Output 08

typedef union tag_DY_DATA_09 {
	DWORD nValue;
	struct {
		DWORD oBufferStage1Vacuum : 1;		// Y0900
		DWORD o0901 : 1;					// Y0901
		DWORD o0902 : 1;					// Y0902
		DWORD oBufferStage1Rotate : 1;		// Y0903
		DWORD oBufferStage1Up : 1;			// Y0904
		DWORD oBufferStage1Down : 1;		// Y0905
		DWORD o0906 : 1;					// Y0906
		DWORD o0907 : 1;					// Y0907
		DWORD oBufferStage2Vacuum : 1;		// Y0908
		DWORD o0909 : 1;					// Y0909
		DWORD o0910 : 1;					// Y0910
		DWORD oBufferStage2Rotate : 1;		// Y0911
		DWORD oBufferStage2Up : 1;			// Y0912
		DWORD oBufferStage2Down : 1;		// Y0913
		DWORD o0914 : 1;					// Y0914
		DWORD o0915 : 1;					// Y0915
		DWORD oBtm1BlowPower : 1;			// Y0916
		DWORD oBtm1BlowOnOff : 1;			// Y0917
		DWORD oBtm1SuctionOn : 1;			// Y0918
		DWORD oBtm1SuctionRelease : 1;		// Y0919
		DWORD oTopBlowPower : 1;			// Y0920
		DWORD oTopBlowOnOff : 1;			// Y0921
		DWORD oTopSuctionOn : 1;			// Y0922
		DWORD oTopSuctionRelease : 1;		// Y0923
		DWORD o0924 : 1;					// Y0924
		DWORD o0925 : 1;					// Y0925
		DWORD oNgPortSlideLock : 1;			// Y0926
		DWORD oNgPortSlideUnlock : 1;		// Y0927
		DWORD o0928 : 1;					// Y0928
		DWORD o0929 : 1;					// Y0929
		DWORD o0930 : 1;					// Y0930
		DWORD o0931 : 1;					// Y0931
	};
} DY_DATA_09;		// Digital Output 09

typedef union tag_DY_DATA_10 {
	DWORD nValue;
	struct {
		DWORD o1000 : 1;					// Y1000
		DWORD oSortPicker1Down1 : 1;		// Y1001
		DWORD o1002 : 1;					// Y1002
		DWORD oSortPicker1Down2 : 1;		// Y1003
		DWORD o1004 : 1;					// Y1004
		DWORD oSortPicker1Down3 : 1;		// Y1005
		DWORD o1006 : 1;					// Y1006
		DWORD oSortPicker1Down4 : 1;		// Y1007
		DWORD o1008 : 1;					// Y1008
		DWORD o1009 : 1;					// Y1009
		DWORD o1010 : 1;					// Y1010
		DWORD o1011 : 1;					// Y1011
		DWORD o1012 : 1;					// Y1012
		DWORD o1013 : 1;					// Y1013
		DWORD o1014 : 1;					// Y1014
		DWORD o1015 : 1;					// Y1015
		DWORD oSortPicker1Open1 : 1;		// Y1016
		DWORD oSortPicker1Close1 : 1;		// Y1017
		DWORD oSortPicker1Open2 : 1;		// Y1018
		DWORD oSortPicker1Close2 : 1;		// Y1019
		DWORD oSortPicker1Open3 : 1;		// Y1020
		DWORD oSortPicker1Close3 : 1;		// Y1021
		DWORD oSortPicker1Open4 : 1;		// Y1022
		DWORD oSortPicker1Close4 : 1;		// Y1023
		DWORD o1024 : 1;					// Y1024
		DWORD o1025 : 1;					// Y1025
		DWORD o1026 : 1;					// Y1026
		DWORD oNgBufferVac01 : 1;			// Y1027
		DWORD oNgBufferVac02 : 1;			// Y1028
		DWORD oNgBufferVac03 : 1;			// Y1029
		DWORD oNgBufferVac04 : 1;			// Y1030
		DWORD o1031 : 1;			// Y1031
	};
} DY_DATA_10;		// Digital Output 10

typedef union tag_DY_DATA_11 {
	DWORD nValue;
	struct {
		DWORD o1100 : 1;					// Y1100
		DWORD oSortPicker2Down1 : 1;		// Y1101
		DWORD o1102 : 1;					// Y1102
		DWORD oSortPicker2Down2 : 1;		// Y1103
		DWORD o1104 : 1;					// Y1104
		DWORD oSortPicker2Down3 : 1;		// Y1105
		DWORD o1106 : 1;					// Y1106
		DWORD oSortPicker2Down4 : 1;		// Y1107
		DWORD o1108 : 1;					// Y1108
		DWORD o1109 : 1;					// Y1109
		DWORD o1110 : 1;					// Y1110
		DWORD o1111 : 1;					// Y1111
		DWORD o1112 : 1;					// Y1112
		DWORD o1113 : 1;					// Y1113
		DWORD o1114 : 1;					// Y1114
		DWORD o1115 : 1;					// Y1115
		DWORD oSortPicker2Open1 : 1;		// Y1116
		DWORD oSortPicker2Close1 : 1;		// Y1117
		DWORD oSortPicker2Open2 : 1;		// Y1118
		DWORD oSortPicker2Close2 : 1;		// Y1119
		DWORD oSortPicker2Open3 : 1;		// Y1120
		DWORD oSortPicker2Close3 : 1;		// Y1121
		DWORD oSortPicker2Open4 : 1;		// Y1122
		DWORD oSortPicker2Close4 : 1;		// Y1123
		DWORD o1124 : 1;					// Y1124
		DWORD o1125 : 1;					// Y1125
		DWORD o1126 : 1;					// Y1126
		DWORD oNgBufferVac05 : 1;			// Y1127
		DWORD oNgBufferVac06 : 1;			// Y1128
		DWORD oNgBufferVac07 : 1;			// Y1129
		DWORD oNgBufferVac08 : 1;			// Y1130
		DWORD o1131 : 1;					// Y1131
	};
} DY_DATA_11;	// Digital Output 11

typedef union tag_DY_DATA_12 {
	DWORD nValue;
	struct {
		DWORD o1200 : 1;					// Y1200
		DWORD o1201 : 1;					// Y1201
		DWORD o1202 : 1;					// Y1202
		DWORD o1203 : 1;					// Y1203
		DWORD o1204 : 1;					// Y1204
		DWORD o1205 : 1;					// Y1205
		DWORD o1206 : 1;					// Y1206
		DWORD o1207 : 1;					// Y1207
		DWORD oGoodPortSlideLock : 1;		// Y1208
		DWORD oGoodPortSlideUnlock : 1;		// Y1209
		DWORD o1210 : 1;					// Y1210
		DWORD o1211 : 1;					// Y1211
		DWORD oGoodTrayBuffSupport1In : 1;	// Y1212
		DWORD o1213 : 1;					// Y1213
		DWORD oGoodTrayBuffSupport2In : 1;	// Y1214
		DWORD o1215 : 1;					// Y1215
		DWORD o1216 : 1;					// Y1216
		DWORD o1217 : 1;					// Y1217
		DWORD o1218 : 1;					// Y1218
		DWORD o1219 : 1;					// Y1219
		DWORD oGoodStage1MasterIn : 1;		// Y1220
		DWORD o1221 : 1;					// Y1221
		DWORD oGoodStage1SlaveIn : 1;		// Y1222
		DWORD o1223 : 1;					// Y1223
		DWORD o1224 : 1;					// Y1224
		DWORD o1225 : 1;					// Y1225
		DWORD o1226 : 1;					// Y1226
		DWORD o1227 : 1;					// Y1227
		DWORD oGoodStage2MasterIn : 1;		// Y1228
		DWORD o1229 : 1;					// Y1229
		DWORD oGoodStage2SlaveIn : 1;		// Y1230
		DWORD o1231 : 1;					// Y1231
	};
} DY_DATA_12;	// Digital Output 12

typedef union tag_DY_DATA_13 {
	DWORD nValue;
	struct {
		DWORD o1300 : 1;					// Y1300
		DWORD o1301 : 1;					// Y1301
		DWORD o1302 : 1;					// Y1302
		DWORD o1303 : 1;					// Y1303
		DWORD o1304 : 1;					// Y1304
		DWORD o1305 : 1;					// Y1305
		DWORD o1306 : 1;					// Y1306
		DWORD o1307 : 1;					// Y1307
		DWORD oEmptyTrans1Up : 1;			// Y1308
		DWORD oEmptyTrans1Down : 1;			// Y1309
		DWORD oEmptyTrans1MasterIn : 1;		// Y1310
		DWORD oEmptyTrans1MasterOut : 1;	// Y1311
		DWORD oEmptyTrans1SlaveIn : 1;		// Y1312
		DWORD oEmptyTrans1SlaveOut : 1;		// Y1313
		DWORD o1314 : 1;					// Y1314
		DWORD o1315 : 1;					// Y1315
		DWORD o1316 : 1;					// Y1316
		DWORD o1317 : 1;					// Y1317
		DWORD o1318 : 1;					// Y1318
		DWORD o1319 : 1;					// Y1319
		DWORD o1320 : 1;					// Y1320
		DWORD o1321 : 1;					// Y1321
		DWORD o1322 : 1;					// Y1322
		DWORD o1323 : 1;					// Y1323
		DWORD oEmptyTrans2Up : 1;			// Y1324
		DWORD oEmptyTrans2Down : 1;			// Y1325
		DWORD oEmptyTrans2MasterIn : 1;		// Y1326
		DWORD oEmptyTrans2MasterOut : 1;	// Y1327
		DWORD oEmptyTrans2SlaveIn : 1;		// Y1328
		DWORD oEmptyTrans2SlaveOut : 1;		// Y1329
		DWORD o1330 : 1;					// Y1330
		DWORD o1331 : 1;					// Y1331
	};
} DY_DATA_13;	// Digital Output 13

typedef union tag_DY_DATA_14 {
	DWORD nValue;
	struct {
		DWORD oTowerGreen : 1;				// Y1400
		DWORD oTowerYellow : 1;				// Y1401
		DWORD oTowerRed : 1;				// Y1402
		DWORD oBuzzerBit0 : 1;				// Y1403
		DWORD oBuzzerBit1 : 1;				// Y1404
		DWORD oBuzzerBit2 : 1;				// Y1405
		DWORD oBuzzerBit3 : 1;				// Y1406
		DWORD oBuzzerBit4 : 1;				// Y1407
		DWORD o1408 : 1;					// Y1408
		DWORD o1409 : 1;					// Y1409
		DWORD o1410 : 1;					// Y1410
		DWORD o1411 : 1;					// Y1411
		DWORD oStartLamp1 : 1;				// Y1412
		DWORD oStartLamp2 : 1;				// Y1413
		DWORD oStartLamp3  : 1;				// Y1414
		DWORD oStopLamp1 : 1;				// Y1415
		DWORD oStopLamp2 : 1;				// Y1416
		DWORD oStopLamp3 : 1;				// Y1417
		DWORD oResetLamp1 : 1;				// Y1418
		DWORD oResetLamp2 : 1;				// Y1419
		DWORD oResetLamp3 : 1;				// Y1420
		DWORD o1421 : 1;					// Y1421
		DWORD o1422 : 1;					// Y1422
		DWORD o1423 : 1;					// Y1423
		DWORD oLoad1Lamp : 1;				// Y1424
		DWORD oLoad2Lamp  : 1;				// Y1425
		DWORD oNgLamp : 1;					// Y1426
		DWORD oGoodLamp : 1;				// Y1427
		DWORD oEmptyLamp : 1;				// Y1428
		DWORD o1429 : 1;					// Y1429
		DWORD o1430 : 1;					// Y1430
		DWORD o1431 : 1;					// Y1431
	};
} DY_DATA_14;	// Digital Output 14

typedef union tag_DY_DATA_15 {
	DWORD nValue;
	struct {
		DWORD oDoor01Unlock : 1;			// Y1500
		DWORD oDoor02Unlock : 1;			// Y1501
		DWORD oDoor03Unlock : 1;			// Y1502
		DWORD oDoor04Unlock : 1;			// Y1503
		DWORD oDoor05Unlock : 1;			// Y1504
		DWORD oDoor06Unlock : 1;			// Y1505
		DWORD oDoor07Unlock : 1;			// Y1506
		DWORD oDoor08Unlock : 1;			// Y1507
		DWORD oDoor09Unlock : 1;			// Y1508
		DWORD oDoor10Unlock : 1;			// Y1509
		DWORD oDoor11Unlock : 1;			// Y1510
		DWORD oDoor12Unlock : 1;			// Y1511
		DWORD oDoor13Unlock : 1;			// Y1512
		DWORD oDoor14Unlock : 1;			// Y1513
		DWORD oDoor15Unlock : 1;			// Y1514
		DWORD oDoor16Unlock : 1;			// Y1515
		DWORD oDoor17Unlock : 1;			// Y1516
		DWORD oDoor18Unlock : 1;			// Y1517
		DWORD oDoor19Unlock : 1;			// Y1518
		DWORD oDoor20Unlock : 1;			// Y1519
		DWORD o1520 : 1;					// Y1520
		DWORD o1521 : 1;					// Y1521
		DWORD o1522 : 1;					// Y1522
		DWORD o1523 : 1;					// Y1523
		DWORD oInsideLight : 1;				// Y1524
		DWORD oSafetyReset : 1;				// Y1525
		DWORD oModeSelect  : 1;				// Y1526
		DWORD o1527 : 1;					// Y1527
		DWORD o1528 : 1;					// Y1528
		DWORD o1529 : 1;					// Y1529
		DWORD o1530 : 1;					// Y1530
		DWORD o1531 : 1;					// Y1531
	};
} DY_DATA_15;	// Digital Output 15
#else
// Input (X0000 - X1031)
typedef union tag_DX_DATA_00 {
	DWORD nValue;
	struct {

		DWORD iLoadPort1Upper : 1;			// X0000
		DWORD iLoadPort1Lower : 1;			// X0001
		DWORD iLoadPort1Bottom : 1;			// X0002
		DWORD i0003 : 1;					// X0003
		DWORD iLoadPort1SlideClose : 1;		// X0004
		DWORD iLoadPort1SlideOpen : 1;		// X0005
		DWORD iLoadPortAreaCheck : 1;		// X0006
		DWORD i0007 : 1;					// X0007
		DWORD iLoadPort1Support1In : 1;		// X0008
		DWORD iLoadPort1Support1Out : 1;	// X0009
		DWORD iLoadPort1Support2In : 1;		// X0010
		DWORD iLoadPort1Support2Out : 1;	// X0011
		DWORD iLoadPort1SlideLock : 1;		// X0012
		DWORD iLoadPort1SlideUnlock : 1;	// X0013
		DWORD i0014 : 1;					// X0014
		DWORD i0015 : 1;					// X0015
		DWORD iLoadPort2Upper : 1;			// X0016
		DWORD iLoadPort2Lower : 1;			// X0017
		DWORD iLoadPort2Bottom : 1;			// X0018
		DWORD i0019 : 1;					// X0019
		DWORD iLoadPort2SlideClose : 1;		// X0020
		DWORD iLoadPort2SlideOpen : 1;		// X0021
		DWORD i0022 : 1;					// X0022
		DWORD i0023 : 1;					// X0023
		DWORD iLoadPort2Support1In : 1;		// X0024
		DWORD iLoadPort2Support1Out : 1;	// X0025
		DWORD iLoadPort2Support2In : 1;		// X0026
		DWORD iLoadPort2Support2Out : 1;	// X0027
		DWORD iLoadPort2SlideLock : 1;		// X0028
		DWORD iLoadPort2SlideUnlock : 1;	// X0029
		DWORD i0030 : 1;					// X0030
		DWORD i0031 : 1;					// X0031
	};
} DX_DATA_00;	// Digital Input 00

typedef union tag_DX_DATA_01 {
	DWORD nValue;
	struct {
		DWORD i0100 : 1;					// X0100
		DWORD iLoadStageExist : 1;			// X0101
		DWORD i0102 : 1;					// X0102
		DWORD i0103 : 1;					// X0103
		DWORD iLoadStageMasterIn : 1;		// X0104
		DWORD iLoadStageMasterOut : 1;		// X0105
		DWORD iLoadStageSlaveIn : 1;		// X0106
		DWORD iLoadStageSlaveOut : 1;		// X0107
		DWORD i0108 : 1;					// X0108
		DWORD i0109 : 1;					// X0109
		DWORD i0110 : 1;					// X0110
		DWORD i0111 : 1;					// X0111
		DWORD i0112 : 1;					// X0112
		DWORD i0113 : 1;					// X0113
		DWORD i0114 : 1;					// X0114
		DWORD i0115 : 1;					// X0115
		DWORD i0116 : 1;					// X0116
		DWORD iLoadPickerExist : 1;			// X0117
		DWORD i0118 : 1;					// X0118
		DWORD i0119 : 1;					// X0119
		DWORD i0120 : 1;					// X0120
		DWORD i0121 : 1;					// X0121
		DWORD i0122 : 1;					// X0122
		DWORD i0123 : 1;					// X0123
		DWORD iLoadPickerUp : 1;			// X0124
		DWORD iLoadPickerDown : 1;			// X0125
		DWORD iLoadPickerMasterIn : 1;		// X0126
		DWORD iLoadPickerMasterOut : 1;		// X0127
		DWORD iLoadPickerSlaveIn : 1;		// X0128
		DWORD iLoadPickerSlaveOut : 1;		// X0129
		DWORD i0130 : 1;					// X0130
		DWORD i0131 : 1;					// X0131
	};
} DX_DATA_01;	// Digital Input 01

typedef union tag_DX_DATA_02 {
	DWORD nValue;
	struct {
		DWORD iAnglePortExist : 1;			// X0200
		DWORD i0201 : 1;					// X0201
		DWORD iAnglePortSupport1In : 1;		// X0202
		DWORD iAnglePortSupport1Out : 1;	// X0203
		DWORD iAnglePortSupport2In : 1;		// X0204
		DWORD iAnglePortSupport2Out : 1;	// X0205
		DWORD iAnglePortMasterIn : 1;		// X0206
		DWORD iAnglePortMasterOut : 1;		// X0207
		DWORD iAnglePortSlaveIn : 1;		// X0208
		DWORD iAnglePortSlaveOut : 1;		// X0209
		DWORD i0210 : 1;					// X0210
		DWORD i0211 : 1;					// X0211
		DWORD i0212 : 1;					// X0212
		DWORD i0213 : 1;					// X0213
		DWORD i0214 : 1;					// X0214
		DWORD i0215 : 1;					// X0215
		DWORD i0216 : 1;					// X0216	//iAngleStage1Vacuum : 1;
		DWORD i0217 : 1;					// X0217
		DWORD iAngleStage1Normal : 1;		// X0218
		DWORD iAngleStage1Rotate : 1;		// X0219
		DWORD iAngleStage1AlignOut : 1;		// X0220
		DWORD iAngleStage1AlignIn : 1;		// X0221
		DWORD iAngleStage1Exist : 1;		// X0222
		DWORD i0223 : 1;					// X0223
		DWORD i0224 : 1;					// X0224	//iAngleStage2Vacuum : 1;
		DWORD i0225 : 1;					// X0225
		DWORD iAngleStage2Normal : 1;		// X0226
		DWORD iAngleStage2Rotate : 1;		// X0227
		DWORD iAngleStage2AlignOut : 1;		// X0228
		DWORD iAngleStage2AlignIn : 1;		// X0229
		DWORD iAngleStage2Exist : 1;		// X0230
		DWORD i0231 : 1;					// X0231
	};
} DX_DATA_02;	// Digital Input 02

typedef union tag_DX_DATA_03 {
	DWORD nValue;
	struct {
		DWORD iBtm1PickerNor01 : 1;			// X0300
		DWORD iBtm1PickerRot01 : 1;			// X0301
		DWORD iBtm1PickerNor02 : 1;			// X0302
		DWORD iBtm1PickerRot02 : 1;			// X0303
		DWORD iBtm1PickerNor03 : 1;			// X0304
		DWORD iBtm1PickerRot03 : 1;			// X0305
		DWORD iBtm1PickerNor04 : 1;			// X0306
		DWORD iBtm1PickerRot04 : 1;			// X0307
		DWORD iBtm1PickerNor05 : 1;			// X0308
		DWORD iBtm1PickerRot05 : 1;			// X0309
		DWORD iBtm1PickerNor06 : 1;			// X0310
		DWORD iBtm1PickerRot06 : 1;			// X0311
		DWORD iBtm1PickerNor07 : 1;			// X0312
		DWORD iBtm1PickerRot07 : 1;			// X0313
		DWORD iBtm1PickerNor08 : 1;			// X0314
		DWORD iBtm1PickerRot08 : 1;			// X0315
		DWORD iBtm1PickerNor09 : 1;			// X0316
		DWORD iBtm1PickerRot09 : 1;			// X0317
		DWORD iBtm1PickerNor10 : 1;			// X0318
		DWORD iBtm1PickerRot10 : 1;			// X0319
		DWORD i0320 : 1;					// X0320
		DWORD i0321 : 1;					// X0321
		DWORD i0322 : 1;					// X0322
		DWORD i0323 : 1;					// X0323
		DWORD iBtm1PickerUp02 : 1;			// X0324
		DWORD iBtm1PickerDown02 : 1;			// X0325
		DWORD iBtm1PickerUp04 : 1;			// X0326
		DWORD iBtm1PickerDown04 : 1;			// X0327
		DWORD iBtm1PickerUp07 : 1;			// X0328
		DWORD iBtm1PickerDown07 : 1;			// X0329
		DWORD iBtm1PickerUp09 : 1;			// X0330
		DWORD iBtm1PickerDown09 : 1;			// X0331
	};
} DX_DATA_03;	// Digital Input 03

typedef union tag_DX_DATA_04 {
	DWORD nValue;
	struct {
		DWORD iBtm1PickerVac01 : 1;			// X0400
		DWORD i0401 : 1;					// X0401
		DWORD iBtm1PickerVac02 : 1;			// X0402
		DWORD i0403 : 1;					// X0403
		DWORD iBtm1PickerVac03 : 1;			// X0404
		DWORD i0405 : 1;					// X0405
		DWORD iBtm1PickerVac04 : 1;			// X0406
		DWORD i0407 : 1;					// X0407
		DWORD iBtm1PickerVac05 : 1;			// X0408
		DWORD i0409 : 1;					// X0409
		DWORD iBtm1PickerVac06 : 1;			// X0410
		DWORD i0411 : 1;					// X0411
		DWORD iBtm1PickerVac07 : 1;			// X0412
		DWORD i0413 : 1;					// X0413
		DWORD iBtm1PickerVac08 : 1;			// X0414
		DWORD i0415 : 1;					// X0415
		DWORD iBtm1PickerVac09 : 1;			// X0416
		DWORD i0417 : 1;					// X0417
		DWORD iBtm1PickerVac10 : 1;			// X0418
		DWORD i0419 : 1;					// X0419
		DWORD i0420 : 1;					// X0420
		DWORD i0421 : 1;					// X0421
		DWORD i0422 : 1;					// X0422
		DWORD i0423 : 1;					// X0423
		DWORD i0424 : 1;					// X0424
		DWORD i0425 : 1;					// X0425
		DWORD i0426 : 1;					// X0426
		DWORD i0427 : 1;					// X0427
		DWORD i0428 : 1;					// X0428
		DWORD i0429 : 1;					// X0429
		DWORD i0430 : 1;					// X0430
		DWORD i0431 : 1;					// X0431
	};
} DX_DATA_04;	// Digital Input 04

typedef union tag_DX_DATA_05 {
	DWORD nValue;
	struct {
		DWORD iInspectStage1Up : 1;			// X0500
		DWORD iInspectStage1Down : 1;		// X0501
		DWORD i0502 : 1;					// X0502
		DWORD i0503 : 1;					// X0503
		DWORD iInspectStage2Up : 1;			// X0504
		DWORD iInspectStage2Down : 1;		// X0505
		DWORD iInspectStage2Fwd : 1;		// X0506
		DWORD iInspectStage2Bwd : 1;		// X0507
		DWORD iInspectStage3Up : 1;			// X0508
		DWORD iInspectStage3Down : 1;		// X0509
		DWORD iInspectStage3Fwd : 1;		// X0510
		DWORD iInspectStage3Bwd : 1;		// X0511
		DWORD i0512 : 1;					// X0512
		DWORD i0513 : 1;					// X0513
		DWORD i0514 : 1;					// X0514
		DWORD i0515 : 1;					// X0515
		DWORD iInspectStage1Vac01 : 1;		// X0516
		DWORD iInspectStage1Vac02 : 1;		// X0517
		DWORD iInspectStage1Vac03 : 1;		// X0518
		DWORD iInspectStage1Vac04 : 1;		// X0519
		DWORD iInspectStage1Vac05 : 1;		// X0520
		DWORD iInspectStage1Vac06 : 1;		// X0521
		DWORD iInspectStage1Vac07 : 1;		// X0522
		DWORD iInspectStage1Vac08 : 1;		// X0523
		DWORD iInspectStage1Vac09 : 1;		// X0524
		DWORD iInspectStage1Vac10 : 1;		// X0525
		DWORD i0526 : 1;					// X0526
		DWORD i0527 : 1;					// X0527
		DWORD i0528 : 1;					// X0528
		DWORD i0529 : 1;					// X0529
		DWORD i0530 : 1;					// X0530
		DWORD i0531 : 1;					// X0531
	};
} DX_DATA_05;	// Digital Input 05

typedef union tag_DX_DATA_06 {
	DWORD nValue;
	struct {
		DWORD iInspectStage2Vac01 : 1;		// X0600
		DWORD iInspectStage2Vac02 : 1;		// X0601
		DWORD iInspectStage2Vac03 : 1;		// X0602
		DWORD iInspectStage2Vac04 : 1;		// X0603
		DWORD iInspectStage2Vac05 : 1;		// X0604
		DWORD iInspectStage2Vac06 : 1;		// X0605
		DWORD iInspectStage2Vac07 : 1;		// X0606
		DWORD iInspectStage2Vac08 : 1;		// X0607
		DWORD iInspectStage2Vac09 : 1;		// X0608
		DWORD iInspectStage2Vac10 : 1;		// X0609
		DWORD i0610 : 1;					// X0610
		DWORD i0611 : 1;					// X0611
		DWORD iCmAlignSlaveClose  : 1;		// X0612
		DWORD iCmAlignSlaveOpen	  : 1;		// X0613
		DWORD i0614 : 1;					// X0614
		DWORD i0615 : 1;					// X0615
		DWORD iInspectStage3Vac01 : 1;		// X0616
		DWORD iInspectStage3Vac02 : 1;		// X0617
		DWORD iInspectStage3Vac03 : 1;		// X0618
		DWORD iInspectStage3Vac04 : 1;		// X0619
		DWORD iInspectStage3Vac05 : 1;		// X0620
		DWORD iInspectStage3Vac06 : 1;		// X0621
		DWORD iInspectStage3Vac07 : 1;		// X0622
		DWORD iInspectStage3Vac08 : 1;		// X0623
		DWORD iInspectStage3Vac09 : 1;		// X0624
		DWORD iInspectStage3Vac10 : 1;		// X0625
		DWORD iCmAlignMasterClose : 1;		// X0626
		DWORD iCmAlignMasterOpen  : 1;		// X0627
		DWORD iTop1Mirror1Up : 1;			// X0628
		DWORD iTop1Mirror1Down : 1;			// X0629
		DWORD iTop1Mirror2Up : 1;			// X0630
		DWORD iTop1Mirror2Down : 1;			// X0631
	};
} DX_DATA_06;	// Digital Input 06

typedef union tag_DX_DATA_07 {
	DWORD nValue;
	struct {
		DWORD iBtm2PickerNor01 : 1;			// X0700
		DWORD iBtm2PickerRot01 : 1;			// X0701
		DWORD iBtm2PickerNor02 : 1;			// X0702
		DWORD iBtm2PickerRot02 : 1;			// X0703
		DWORD iBtm2PickerNor03 : 1;			// X0704
		DWORD iBtm2PickerRot03 : 1;			// X0705
		DWORD iBtm2PickerNor04 : 1;			// X0706
		DWORD iBtm2PickerRot04 : 1;			// X0707
		DWORD iBtm2PickerNor05 : 1;			// X0708
		DWORD iBtm2PickerRot05 : 1;			// X0709
		DWORD iBtm2PickerNor06 : 1;			// X0710
		DWORD iBtm2PickerRot06 : 1;			// X0711
		DWORD iBtm2PickerNor07 : 1;			// X0712
		DWORD iBtm2PickerRot07 : 1;			// X0713
		DWORD iBtm2PickerNor08 : 1;			// X0714
		DWORD iBtm2PickerRot08 : 1;			// X0715
		DWORD iBtm2PickerNor09 : 1;			// X0716
		DWORD iBtm2PickerRot09 : 1;			// X0717
		DWORD iBtm2PickerNor10 : 1;			// X0718
		DWORD iBtm2PickerRot10 : 1;			// X0719
		DWORD i0720 : 1;					// X0720
		DWORD i0721 : 1;					// X0721
		DWORD i0722 : 1;					// X0722
		DWORD i0723 : 1;					// X0723
		DWORD iEmptyPortTopCheck : 1;		// X0724
		DWORD i0725 : 1;					// X0725
		DWORD iEmptyPortSlideClose : 1;		// X0726
		DWORD iEmptyPortSlideOpen : 1;		// X0727
		DWORD iEmptyPortAreaCheck : 1;		// X0728
		DWORD i0729 : 1;					// X0729
		DWORD iEmptyPortExist : 1;			// X0730
		DWORD i0731 : 1;					// X0731
	};
} DX_DATA_07;	// Digital Input 07

typedef union tag_DX_DATA_08 {
	DWORD nValue;
	struct {
		DWORD iBtm2PickerVac01 : 1;			// X0800
		DWORD i0801 : 1;					// X0801
		DWORD iBtm2PickerVac02 : 1;			// X0802
		DWORD i0803 : 1;					// X0803
		DWORD iBtm2PickerVac03 : 1;			// X0804
		DWORD i0805 : 1;					// X0805
		DWORD iBtm2PickerVac04 : 1;			// X0806
		DWORD i0807 : 1;					// X0807
		DWORD iBtm2PickerVac05 : 1;			// X0808
		DWORD i0809 : 1;					// X0809
		DWORD iBtm2PickerVac06 : 1;			// X0810
		DWORD i0811 : 1;					// X0811
		DWORD iBtm2PickerVac07 : 1;			// X0812
		DWORD i0813 : 1;					// X0813
		DWORD iBtm2PickerVac08 : 1;			// X0814
		DWORD i0815 : 1;					// X0815
		DWORD iBtm2PickerVac09 : 1;			// X0816
		DWORD i0817 : 1;					// X0817
		DWORD iBtm2PickerVac10 : 1;			// X0818
		DWORD i0819 : 1;					// X0819
		DWORD i0820 : 1;					// X0820
		DWORD i0821 : 1;					// X0821
		DWORD i0822 : 1;					// X0822
		DWORD i0823 : 1;					// X0823
		DWORD iBtm2PickerUp02 : 1;			// X0824
		DWORD iBtm2PickerDown02 : 1;		// X0825
		DWORD iBtm2PickerUp04 : 1;			// X0826
		DWORD iBtm2PickerDown04 : 1;		// X0827
		DWORD iBtm2PickerUp07 : 1;			// X0828
		DWORD iBtm2PickerDown07 : 1;		// X0829
		DWORD iBtm2PickerUp09 : 1;			// X0830
		DWORD iBtm2PickerDown09 : 1;		// X0831
	};
} DX_DATA_08;	// Digital Input 08

typedef union tag_DX_DATA_09 {
	DWORD nValue;
	struct {
		DWORD iBufferStage1Vacuum : 1;		// X0900
		DWORD i0901 : 1;					// X0901
		DWORD iBufferStage1Normal : 1;		// X0902
		DWORD iBufferStage1Rotate : 1;		// X0903
		DWORD iBufferStage1Up : 1;			// X0904
		DWORD iBufferStage1Down : 1;		// X0905
		DWORD i0906 : 1;					// X0906
		DWORD i0907 : 1;					// X0907
		DWORD iBufferStage2Vacuum : 1;		// X0908
		DWORD i0909 : 1;					// X0909
		DWORD iBufferStage2Normal : 1;		// X0910
		DWORD iBufferStage2Rotate : 1;		// X0911
		DWORD iBufferStage2Up : 1;			// X0912
		DWORD iBufferStage2Down : 1;		// X0913
		DWORD i0914 : 1;					// X0914
		DWORD i0915 : 1;					// X0915
		DWORD iNgStageExist1 : 1;			// X0916
		DWORD iNgStageExist2 : 1;			// X0917
		DWORD iNgStageExist3 : 1;			// X0918
		DWORD iNgStageExist4 : 1;			// X0919
		DWORD i0920 : 1;					// X0920
		DWORD i0921 : 1;					// X0921
		DWORD i0922 : 1;					// X0922
		DWORD i0923 : 1;					// X0923
		DWORD iNgPortSlideClose : 1;		// X0924
		DWORD iNgPortSlideOpen : 1;			// X0925
		DWORD iNgPortSlideLock : 1;			// X0926
		DWORD iNgPortSlideUnlock : 1;		// X0927
		DWORD i0928 : 1;					// X0928
		DWORD i0929 : 1;					// X0929
		DWORD iNgPortAreaCheck : 1;			// X0930
		DWORD i0931 : 1;					// X0931
	};
} DX_DATA_09;	// Digital Input 09

typedef union tag_DX_DATA_10 {
	DWORD nValue;
	struct {
		DWORD iSortPicker1Up1 : 1;			// X1000
		DWORD iSortPicker1Down1 : 1;		// X1001
		DWORD iSortPicker1Up2 : 1;			// X1002
		DWORD iSortPicker1Down2 : 1;		// X1003
		DWORD iSortPicker1Up3 : 1;			// X1004
		DWORD iSortPicker1Down3 : 1;		// X1005
		DWORD iSortPicker1Up4 : 1;			// X1006
		DWORD iSortPicker1Down4 : 1;		// X1007
		DWORD iSortPicker1Up5 : 1;			// X1008
		DWORD iSortPicker1Down5 : 1;		// X1009
		DWORD i1010 : 1;					// X1010
		DWORD i1011 : 1;					// X1011
		DWORD i1012 : 1;					// X1012
		DWORD i1013 : 1;					// X1013
		DWORD i1014 : 1;					// X1014
		DWORD i1015 : 1;					// X1015
		DWORD iSortPicker1Vac1 : 1;			// X1016
		DWORD i1017 : 1;					// X1017
		DWORD iSortPicker1Vac2 : 1;			// X1018
		DWORD i1019 : 1;					// X1019
		DWORD iSortPicker1Vac3 : 1;			// X1020
		DWORD i1021 : 1;					// X1021
		DWORD iSortPicker1Vac4 : 1;			// X1022
		DWORD i1023 : 1;					// X1023
		DWORD iSortPicker1Vac5 : 1;			// X1024
		DWORD i1025 : 1;					// X1025
		DWORD i1026 : 1;					// X1026
		DWORD iNgBufferVac01 : 1;			// X1027
		DWORD iNgBufferVac02 : 1;			// X1028
		DWORD iNgBufferVac03 : 1;			// X1029
		DWORD iNgBufferVac04 : 1;			// X1030
		DWORD iNgBufferVac05 : 1;			// X1031
	};
} DX_DATA_10;	// Digital Input 10

typedef union tag_DX_DATA_11 {
	DWORD nValue;
	struct {
		DWORD iSortPicker2Up1 : 1;			// X1100
		DWORD iSortPicker2Down1 : 1;		// X1101
		DWORD iSortPicker2Up2 : 1;			// X1102
		DWORD iSortPicker2Down2 : 1;		// X1103
		DWORD iSortPicker2Up3 : 1;			// X1104
		DWORD iSortPicker2Down3 : 1;		// X1105
		DWORD iSortPicker2Up4 : 1;			// X1106
		DWORD iSortPicker2Down4 : 1;		// X1107
		DWORD iSortPicker2Up5 : 1;			// X1108
		DWORD iSortPicker2Down5 : 1;		// X1109
		DWORD i1110 : 1;					// X1110
		DWORD i1111 : 1;					// X1111
		DWORD i1112 : 1;					// X1112
		DWORD i1113 : 1;					// X1113
		DWORD i1114 : 1;					// X1114
		DWORD i1115 : 1;					// X1115
		DWORD iSortPicker2Vac1 : 1;			// X1116
		DWORD i1117 : 1;					// X1117
		DWORD iSortPicker2Vac2 : 1;			// X1118
		DWORD i1119 : 1;					// X1119
		DWORD iSortPicker2Vac3 : 1;			// X1120
		DWORD i1121 : 1;					// X1121
		DWORD iSortPicker2Vac4 : 1;			// X1122
		DWORD i1123 : 1;					// X1123
		DWORD iSortPicker2Vac5 : 1;			// X1124
		DWORD i1125 : 1;					// X1125
		DWORD i1126 : 1;					// X1126
		DWORD iNgBufferVac06 : 1;			// X1127
		DWORD iNgBufferVac07 : 1;			// X1128
		DWORD iNgBufferVac08 : 1;			// X1129
		DWORD iNgBufferVac09 : 1;			// X1130
		DWORD iNgBufferVac10 : 1;			// X1131
	};
} DX_DATA_11;	// Digital Input 11

typedef union tag_DX_DATA_12 {
	DWORD nValue;
	struct {
		DWORD iGoodPortUpper : 1;			// X1200
		DWORD iGoodPortLower : 1;			// X1201
		DWORD iGoodPortBottom : 1;			// X1202
		DWORD i1203 : 1;					// X1203
		DWORD iGoodPortSlideClose : 1;		// X1204
		DWORD iGoodPortSlideOpen : 1;		// X1205
		DWORD iGoodPortAreaCheck : 1;		// X1206
		DWORD i1207 : 1;					// X1207
		DWORD iGoodPortSlideLock : 1;		// X1208
		DWORD iGoodPortSlideUnlock : 1;		// X1209
		DWORD iGoodTrayBufferBottom : 1;	// X1210
		DWORD i1211 : 1;					// X1211
		DWORD iGoodTrayBuffSupport1In : 1;	// X1212
		DWORD iGoodTrayBuffSupport1Out : 1;	// X1213
		DWORD iGoodTrayBuffSupport2In : 1;	// X1214
		DWORD iGoodTrayBuffSupport2Out : 1;	// X1215
		DWORD i1216 : 1;					// X1216
		DWORD iGoodStage1Exist : 1;			// X1217
		DWORD i1218 : 1;					// X1218
		DWORD i1219 : 1;					// X1219
		DWORD iGoodStage1MasterIn : 1;		// X1220
		DWORD iGoodStage1MasterOut : 1;		// X1221
		DWORD iGoodStage1SlaveIn : 1;		// X1222
		DWORD iGoodStage1SlaveOut : 1;		// X1223
		DWORD i1224 : 1;					// X1224
		DWORD iGoodStage2Exist : 1;			// X1225
		DWORD i1226 : 1;					// X1226
		DWORD i1227 : 1;					// X1227
		DWORD iGoodStage2MasterIn : 1;		// X1228
		DWORD iGoodStage2MasterOut : 1;		// X1229
		DWORD iGoodStage2SlaveIn : 1;		// X1230
		DWORD iGoodStage2SlaveOut : 1;		// X1231
	};
} DX_DATA_12;	// Digital Input 12

typedef union tag_DX_DATA_13 {
	DWORD nValue;
	struct {
		DWORD i1300 : 1;					// X1300
		DWORD iEmptyTrans1Exist : 1;		// X1301
		DWORD i1302 : 1;					// X1302
		DWORD i1303 : 1;					// X1303
		DWORD i1304 : 1;					// X1304
		DWORD i1305 : 1;					// X1305
		DWORD i1306 : 1;					// X1306
		DWORD i1307 : 1;					// X1307
		DWORD iEmptyTrans1Up : 1;			// X1308
		DWORD iEmptyTrans1Down : 1;			// X1309
		DWORD iEmptyTrans1MasterIn : 1;		// X1310
		DWORD iEmptyTrans1MasterOut : 1;	// X1311
		DWORD iEmptyTrans1SlaveIn : 1;		// X1312
		DWORD iEmptyTrans1SlaveOut : 1;		// X1313
		DWORD i1314 : 1;					// X1314
		DWORD i1315 : 1;					// X1315
		DWORD iEmptyTrans2Exist : 1;		// X1316
		DWORD i1317 : 1;					// X1317
		DWORD i1318 : 1;					// X1318
		DWORD i1319 : 1;					// X1319
		DWORD i1320 : 1;					// X1320
		DWORD i1321 : 1;					// X1321
		DWORD i1322 : 1;					// X1322
		DWORD i1323 : 1;					// X1323
		DWORD iEmptyTrans2Up : 1;			// X1324
		DWORD iEmptyTrans2Down : 1;			// X1325
		DWORD iEmptyTrans2MasterIn : 1;		// X1326
		DWORD iEmptyTrans2MasterOut : 1;	// X1327
		DWORD iEmptyTrans2SlaveIn : 1;		// X1328
		DWORD iEmptyTrans2SlaveOut : 1;		// X1329
		DWORD i1330 : 1;					// X1330
		DWORD i1331 : 1;					// X1331
	};
} DX_DATA_13;	// Digital Input 13

typedef union tag_DX_DATA_14 {
	DWORD nValue;
	struct {
		DWORD iEmgSw1 : 1;					// X1400
		DWORD iEmgSw2 : 1;					// X1401
		DWORD iEmgSw3 : 1;					// X1402
		DWORD iEmgSw4 : 1;					// X1403
		DWORD iEmgSw5 : 1;					// X1404
		DWORD iEmgSw6 : 1;					// X1405
		DWORD iMainAir1 : 1;				// X1406
		DWORD iMainAir2 : 1;				// X1407
		DWORD iMainAir3 : 1;				// X1408
		DWORD i1409 : 1;					// X1409
		DWORD i1410 : 1;					// X1410
		DWORD i1411 : 1;					// X1411
		DWORD iStartSw : 1;					// X1412
		DWORD i1413 : 1;					// X1413
		DWORD i1414 : 1;					// X1414
		DWORD iStopSw : 1;					// X1415
		DWORD i1416 : 1;					// X1416
		DWORD i1417 : 1;					// X1417
		DWORD iResetSw : 1;					// X1418
		DWORD i1419 : 1;					// X1419
		DWORD i1420 : 1;					// X1420
		DWORD i1421 : 1;					// X1421
		DWORD i1422 : 1;					// X1422
		DWORD i1423 : 1;					// X1423
		DWORD iLoad1Sw : 1;					// X1424
		DWORD iLoad2Sw : 1;					// X1425
		DWORD iNgSw : 1;					// X1426
		DWORD iGoodSw : 1;					// X1427
		DWORD iEmptySw : 1;					// X1428
		DWORD i1429 : 1;					// X1429
		DWORD i1430 : 1;					// X1430
		DWORD i1431 : 1;					// X1431
	};
} DX_DATA_14;	// Digital Input 14

typedef union tag_DX_DATA_15 {
	DWORD nValue;
	struct {
		DWORD iDoor01Unlock : 1;			// X1500
		DWORD iDoor02Unlock : 1;			// X1501
		DWORD iDoor03Unlock : 1;			// X1502
		DWORD iDoor04Unlock : 1;			// X1503
		DWORD iDoor05Unlock : 1;			// X1504
		DWORD iDoor06Unlock : 1;			// X1505
		DWORD iDoor07Unlock : 1;			// X1506
		DWORD iDoor08Unlock : 1;			// X1507
		DWORD iDoor09Unlock : 1;			// X1508
		DWORD iDoor10Unlock : 1;			// X1509
		DWORD iDoor11Unlock : 1;			// X1510
		DWORD iDoor12Unlock : 1;			// X1511
		DWORD iDoor13Unlock : 1;			// X1512
		DWORD iDoor14Unlock : 1;			// X1513
		DWORD iDoor15Unlock : 1;			// X1514
		DWORD iDoor16Unlock : 1;			// X1515
		DWORD iDoor17Unlock : 1;			// X1516
		DWORD iDoor18Unlock : 1;			// X1517
		DWORD iDoor19Unlock : 1;			// X1518
		DWORD i1519 : 1;					// X1519
		DWORD i1520 : 1;					// X1520
		DWORD i1521 : 1;					// X1521
		DWORD i1522 : 1;					// X1522
		DWORD i1523 : 1;					// X1523
		DWORD i1524 : 1;					// X1524
		DWORD i1525 : 1;					// X1525
		DWORD i1526 : 1;					// X1526
		DWORD i1527 : 1;					// X1527
		DWORD i1528 : 1;					// X1528
		DWORD i1529 : 1;					// X1529
		DWORD i1530 : 1;					// X1530
		DWORD i1531 : 1;					// X1531
	};
} DX_DATA_15;	// Digital Input 15

///////////////////////////////////////////////////////////////////////////////
// Output (Y0000 - Y1331)

typedef union tag_DY_DATA_00 {
	DWORD nValue;
	struct {
		DWORD o0000 : 1;					// Y0000
		DWORD o0001 : 1;					// Y0001
		DWORD o0002 : 1;					// Y0002
		DWORD o0003 : 1;					// Y0003
		DWORD o0004 : 1;					// Y0004
		DWORD o0005 : 1;					// Y0005
		DWORD o0006 : 1;					// Y0006
		DWORD o0007 : 1;					// Y0007
		DWORD oLoadPort1SupportIn : 1;		// Y0008
		DWORD oLoadPort1SupportOut : 1;		// Y0009
		DWORD o0010 : 1;					// Y0010
		DWORD o0011 : 1;					// Y0011
		DWORD oLoadPort1SlideLock : 1;		// Y0012
		DWORD oLoadPort1SlideUnlock : 1;	// Y0013
		DWORD o0014 : 1;					// Y0014
		DWORD o0015 : 1;					// Y0015
		DWORD o0016 : 1;					// Y0016
		DWORD o0017 : 1;					// Y0017
		DWORD o0018 : 1;					// Y0018
		DWORD o0019 : 1;					// Y0019
		DWORD o0020 : 1;					// Y0020
		DWORD o0021 : 1;					// Y0021
		DWORD o0022 : 1;					// Y0022
		DWORD o0023 : 1;					// Y0023
		DWORD oLoadPort2SupportIn : 1;		// Y0024
		DWORD oLoadPort2SupportOut : 1;		// Y0025
		DWORD o0026 : 1;					// Y0026
		DWORD o0027 : 1;					// Y0027
		DWORD oLoadPort2SlideLock : 1;		// Y0028
		DWORD oLoadPort2SlideUnlock : 1;	// Y0029
		DWORD o0030 : 1;					// Y0030
		DWORD o0031 : 1;					// Y0031
	};
} DY_DATA_00;		// Digital Output 00

typedef union tag_DY_DATA_01 {
	DWORD nValue;
	struct {
		DWORD o0100 : 1;					// Y0100
		DWORD o0101 : 1;					// Y0101
		DWORD o0102 : 1;					// Y0102
		DWORD o0103 : 1;					// Y0103
		DWORD oLoadStageMasterIn : 1;		// Y0104
		DWORD o0105 : 1;					// Y0105
		DWORD oLoadStageSlaveIn : 1;		// Y0106
		DWORD o0107 : 1;					// Y0107
		DWORD o0108 : 1;					// Y0108
		DWORD o0109 : 1;					// Y0109
		DWORD o0110 : 1;					// Y0110
		DWORD o0111 : 1;					// Y0111
		DWORD o0112 : 1;					// Y0112
		DWORD o0113 : 1;					// Y0113
		DWORD o0114 : 1;					// Y0114
		DWORD o0115 : 1;					// Y0115
		DWORD o0116 : 1;					// Y0116
		DWORD o0117 : 1;					// Y0117
		DWORD o0118 : 1;					// Y0118
		DWORD o0119 : 1;					// Y0119
		DWORD o0120 : 1;					// Y0120
		DWORD o0121 : 1;					// Y0121
		DWORD o0122 : 1;					// Y0122
		DWORD o0123 : 1;					// Y0123
		DWORD oLoadPickerUp : 1;			// Y0124
		DWORD oLoadPickerDown : 1;			// Y0125
		DWORD oLoadPickerMasterIn : 1;		// Y0126
		DWORD oLoadPickerMasterOut : 1;		// Y0127
		DWORD oLoadPickerSlaveIn : 1;		// Y0128
		DWORD oLoadPickerSlaveOut : 1;		// Y0129
		DWORD o0130 : 1;					// Y0130
		DWORD o0131 : 1;					// Y0131
	};
} DY_DATA_01;		// Digital Output 01

typedef union tag_DY_DATA_02 {
	DWORD nValue;
	struct {
		DWORD o0200 : 1;					// Y0200
		DWORD o0201 : 1;					// Y0201
		DWORD oAnglePortSupportIn : 1;		// Y0202
		DWORD oAnglePortSupportOut : 1;		// Y0203
		DWORD o0204 : 1;					// Y0204
		DWORD o0205 : 1;					// Y0205
		DWORD oAnglePortMasterIn : 1;		// Y0206
		DWORD oAnglePortMasterOut : 1;		// Y0207
		DWORD oAnglePortSlaveIn : 1;		// Y0208
		DWORD oAnglePortSlaveOut : 1;		// Y0209
		DWORD o0210 : 1;					// Y0210
		DWORD o0211 : 1;					// Y0211
		DWORD o0212 : 1;					// Y0212
		DWORD o0213 : 1;					// Y0213
		DWORD o0214 : 1;					// Y0214
		DWORD o0215 : 1;					// Y0215
		DWORD oAngleStage1AlignIn : 1;		// Y0216
		DWORD o0217 : 1;					// Y0217
		DWORD o0218 : 1;					// Y0218
		DWORD oAngleStage1Rotate : 1;		// Y0219
		DWORD o0220 : 1;					// Y0220
		DWORD o0221 : 1;					// Y0221
		DWORD o0222 : 1;					// Y0222
		DWORD o0223 : 1;					// Y0223
		DWORD oAngleStage2AlignIn : 1;		// Y0224
		DWORD o0225 : 1;					// Y0225
		DWORD o0226 : 1;					// Y0226
		DWORD oAngleStage2Rotate : 1;		// Y0227
		DWORD o0228 : 1;					// Y0228
		DWORD o0229 : 1;					// Y0229
		DWORD o0230 : 1;					// Y0230
		DWORD o0231 : 1;					// Y0231
	};
} DY_DATA_02;		// Digital Output 02

typedef union tag_DY_DATA_03 {
	DWORD nValue;
	struct {
		DWORD o0300 : 1;					// Y0300
		DWORD oBtm1PickerDown01 : 1;		// Y0301			// 1, 3, 5번
		DWORD o0302 : 1;					// Y0302
		DWORD oBtm1PickerDown02 : 1;		// Y0303			// 2, 4번
		DWORD o0304 : 1;					// Y0304
		DWORD oBtm1PickerTurn01 : 1;		// Y0305
		DWORD o0306 : 1;					// Y0306
		DWORD oBtm1PickerTurn23 : 1;		// Y0307
		DWORD o0308 : 1;					// Y0308
		DWORD oBtm1PickerTurn45 : 1;		// Y0309
		DWORD o0310 : 1;					// Y0310
		DWORD oBtm1PickerDown06 : 1;		// Y0311			// 6, 8, 10번
		DWORD o0312 : 1;					// Y0312
		DWORD oBtm1PickerDown07 : 1;		// Y0313			// 7, 9번
		DWORD o0314 : 1;					// Y0314
		DWORD oBtm1PickerTurn06 : 1;		// Y0315
		DWORD o0316 : 1;					// Y0316
		DWORD oBtm1PickerTurn78 : 1;		// Y0317
		DWORD o0318 : 1;					// Y0318
		DWORD oBtm1PickerTurn910 : 1;		// Y0319
		DWORD o0320 : 1;					// Y0320
		DWORD o0321 : 1;					// Y0321
		DWORD o0322 : 1;					// Y0322
		DWORD o0323 : 1;					// Y0323
		DWORD o0324 : 1;					// Y0324
		DWORD o0325 : 1;					// Y0325
		DWORD o0326 : 1;					// Y0326
		DWORD o0327 : 1;					// Y0327
		DWORD o0328 : 1;					// Y0328
		DWORD o0329 : 1;					// Y0329
		DWORD o0330 : 1;					// Y0330
		DWORD o0331 : 1;					// Y0331
	};
} DY_DATA_03;		// Digital Output 03

typedef union tag_DY_DATA_04 {
	DWORD nValue;
	struct {
		DWORD oBtm1PickerVac01 : 1;			// Y0400
		DWORD oBtm1PickerAir01 : 1;			// Y0401
		DWORD oBtm1PickerVac02 : 1;			// Y0402
		DWORD oBtm1PickerAir02 : 1;			// Y0403
		DWORD oBtm1PickerVac03 : 1;			// Y0404
		DWORD oBtm1PickerAir03 : 1;			// Y0405
		DWORD oBtm1PickerVac04 : 1;			// Y0406
		DWORD oBtm1PickerAir04 : 1;			// Y0407
		DWORD oBtm1PickerVac05 : 1;			// Y0408
		DWORD oBtm1PickerAir05 : 1;			// Y0409
		DWORD oBtm1PickerVac06 : 1;			// Y0410
		DWORD oBtm1PickerAir06 : 1;			// Y0411
		DWORD oBtm1PickerVac07 : 1;			// Y0412
		DWORD oBtm1PickerAir07 : 1;			// Y0413
		DWORD oBtm1PickerVac08 : 1;			// Y0414
		DWORD oBtm1PickerAir08 : 1;			// Y0415
		DWORD oBtm1PickerVac09 : 1;			// Y0416
		DWORD oBtm1PickerAir09 : 1;			// Y0417
		DWORD oBtm1PickerVac10 : 1;			// Y0418
		DWORD oBtm1PickerAir10 : 1;			// Y0419
		DWORD o0420 : 1;					// Y0420
		DWORD o0421 : 1;					// Y0421
		DWORD o0422 : 1;					// Y0422
		DWORD o0423 : 1;					// Y0423
		DWORD o0424 : 1;					// Y0424
		DWORD o0425 : 1;					// Y0425
		DWORD o0426 : 1;					// Y0426
		DWORD o0427 : 1;					// Y0427
		DWORD o0428 : 1;					// Y0428
		DWORD o0429 : 1;					// Y0429
		DWORD o0430 : 1;					// Y0430
		DWORD o0431 : 1;					// Y0431
	};
} DY_DATA_04;		// Digital Output 04

typedef union tag_DY_DATA_05 {
	DWORD nValue;
	struct {
		DWORD oInspectStage1Up : 1;			// Y0500
		DWORD oInspectStage1Down : 1;		// Y0501
		DWORD o0502 : 1;					// Y0502
		DWORD o0503 : 1;					// Y0503
		DWORD oInspectStage2Up : 1;			// Y0504
		DWORD oInspectStage2Down : 1;		// Y0505
		DWORD oInspectStage2Fwd : 1;		// Y0506
		DWORD oInspectStage2Bwd : 1;		// Y0507
		DWORD oInspectStage3Up : 1;			// Y0508
		DWORD oInspectStage3Down : 1;		// Y0509
		DWORD oInspectStage3Fwd : 1;		// Y0510
		DWORD oInspectStage3Bwd : 1;		// Y0511
		DWORD o0512 : 1;					// Y0512
		DWORD o0513 : 1;					// Y0513
		DWORD o0514 : 1;					// Y0514
		DWORD o0515 : 1;					// Y0515
		DWORD oInspectStage1Vac01 : 1;		// Y0516
		DWORD oInspectStage1Vac02 : 1;		// Y0517
		DWORD oInspectStage1Vac03 : 1;		// Y0518
		DWORD oInspectStage1Vac04 : 1;		// Y0519
		DWORD oInspectStage1Vac05 : 1;		// Y0520
		DWORD oInspectStage1Vac06 : 1;		// Y0521
		DWORD oInspectStage1Vac07 : 1;		// Y0522
		DWORD oInspectStage1Vac08 : 1;		// Y0523
		DWORD oInspectStage1Vac09 : 1;		// Y0524
		DWORD oInspectStage1Vac10 : 1;		// Y0525
		DWORD o0526 : 1;					// Y0526
		DWORD o0527 : 1;					// Y0527
		DWORD o0528 : 1;					// Y0528
		DWORD o0529 : 1;					// Y0529
		DWORD o0530 : 1;					// Y0530
		DWORD o0531 : 1;					// Y0531
	};
} DY_DATA_05;		// Digital Output 05

typedef union tag_DY_DATA_06 {
	DWORD nValue;
	struct {
		DWORD oInspectStage2Vac01 : 1;		// Y0600
		DWORD oInspectStage2Vac02 : 1;		// Y0601
		DWORD oInspectStage2Vac03 : 1;		// Y0602
		DWORD oInspectStage2Vac04 : 1;		// Y0603
		DWORD oInspectStage2Vac05 : 1;		// Y0604
		DWORD oInspectStage2Vac06 : 1;		// Y0605
		DWORD oInspectStage2Vac07 : 1;		// Y0606
		DWORD oInspectStage2Vac08 : 1;		// Y0607
		DWORD oInspectStage2Vac09 : 1;		// Y0608
		DWORD oInspectStage2Vac10 : 1;		// Y0609
		DWORD o0610 : 1;					// Y0610
		DWORD o0611 : 1;					// Y0611
		DWORD oCmAlignSlaveClose  : 1;		// Y0612
		DWORD oCmAlignSlaveOpen   : 1;		// Y0613
		DWORD o0614 : 1;					// Y0614
		DWORD o0615 : 1;					// Y0615
		DWORD oInspectStage3Vac01 : 1;		// Y0616
		DWORD oInspectStage3Vac02 : 1;		// Y0617
		DWORD oInspectStage3Vac03 : 1;		// Y0618
		DWORD oInspectStage3Vac04 : 1;		// Y0619
		DWORD oInspectStage3Vac05 : 1;		// Y0620
		DWORD oInspectStage3Vac06 : 1;		// Y0621
		DWORD oInspectStage3Vac07 : 1;		// Y0622
		DWORD oInspectStage3Vac08 : 1;		// Y0623
		DWORD oInspectStage3Vac09 : 1;		// Y0624
		DWORD oInspectStage3Vac10 : 1;		// Y0625
		DWORD oCmAlignMasterClose : 1;		// Y0626
		DWORD oCmAlignMasterOpen  : 1;		// Y0627
		DWORD oTop1Mirror12Up : 1;			// Y0628
		DWORD oTop1Mirror12Down : 1;		// Y0629
		DWORD o0630 : 1;					// Y0630
		DWORD o0631 : 1;					// Y0631
	};
} DY_DATA_06;		// Digital Output 06

typedef union tag_DY_DATA_07 {
	DWORD nValue;
	struct {
		DWORD o0700 : 1;					// Y0700
		DWORD oBtm2PickerDown01 : 1;		// Y0701			// 1, 3, 5번
		DWORD o0702 : 1;					// Y0702
		DWORD oBtm2PickerDown02 : 1;		// Y0703			// 2, 4번
		DWORD o0704 : 1;					// Y0704
		DWORD oBtm2PickerTurn01  : 1;		// Y0705
		DWORD o0706 : 1;					// Y0706
		DWORD oBtm2PickerTurn23 : 1;		// Y0707
		DWORD o0708 : 1;					// Y0708
		DWORD oBtm2PickerTurn45 : 1;		// Y0709
		DWORD o0710 : 1;					// Y0710
		DWORD oBtm2PickerDown06 : 1;		// Y0711			// 6, 8, 10번
		DWORD o0712 : 1;					// Y0712
		DWORD oBtm2PickerDown07 : 1;		// Y0713			// 7, 9번
		DWORD o0714 : 1;					// Y0714
		DWORD oBtm2PickerTurn67 : 1;		// Y0715
		DWORD o0716 : 1;					// Y0716
		DWORD oBtm2PickerTurn89 : 1;		// Y0717
		DWORD o0718 : 1;					// Y0718
		DWORD oBtm2PickerTurn10 : 1;		// Y0719
		DWORD o0720 : 1;					// Y0720
		DWORD o0721 : 1;					// Y0721
		DWORD o0722 : 1;					// Y0722
		DWORD o0723 : 1;					// Y0723
		DWORD o0724 : 1;					// Y0724
		DWORD o0725 : 1;					// Y0725
		DWORD o0726 : 1;					// Y0726
		DWORD o0727 : 1;					// Y0727
		DWORD o0728 : 1;					// Y0728
		DWORD o0729 : 1;					// Y0729
		DWORD o0730 : 1;					// Y0730
		DWORD o0731 : 1;					// Y0731
	};
} DY_DATA_07;		// Digital Output 07

typedef union tag_DY_DATA_08 {
	DWORD nValue;
	struct {
		DWORD oBtm2PickerVac01 : 1;			// Y0800
		DWORD oBtm2PickerAir01 : 1;			// Y0801
		DWORD oBtm2PickerVac02 : 1;			// Y0802
		DWORD oBtm2PickerAir02 : 1;			// Y0803
		DWORD oBtm2PickerVac03 : 1;			// Y0804
		DWORD oBtm2PickerAir03 : 1;			// Y0805
		DWORD oBtm2PickerVac04 : 1;			// Y0806
		DWORD oBtm2PickerAir04 : 1;			// Y0807
		DWORD oBtm2PickerVac05 : 1;			// Y0808
		DWORD oBtm2PickerAir05 : 1;			// Y0809
		DWORD oBtm2PickerVac06 : 1;			// Y0810
		DWORD oBtm2PickerAir06 : 1;			// Y0811
		DWORD oBtm2PickerVac07 : 1;			// Y0812
		DWORD oBtm2PickerAir07 : 1;			// Y0813
		DWORD oBtm2PickerVac08 : 1;			// Y0814
		DWORD oBtm2PickerAir08 : 1;			// Y0815
		DWORD oBtm2PickerVac09 : 1;			// Y0816
		DWORD oBtm2PickerAir09 : 1;			// Y0817
		DWORD oBtm2PickerVac10 : 1;			// Y0818
		DWORD oBtm2PickerAir10 : 1;			// Y0819
		DWORD o0820 : 1;					// Y0820
		DWORD o0821 : 1;					// Y0821
		DWORD o0822 : 1;					// Y0822
		DWORD o0823 : 1;					// Y0823
		DWORD o0824 : 1;					// Y0824
		DWORD o0825 : 1;					// Y0825
		DWORD o0826 : 1;					// Y0826
		DWORD o0827 : 1;					// Y0827
		DWORD o0828 : 1;					// Y0828
		DWORD o0829 : 1;					// Y0829
		DWORD o0830 : 1;					// Y0830
		DWORD o0831 : 1;					// Y0831
	};
} DY_DATA_08;		// Digital Output 08

typedef union tag_DY_DATA_09 {
	DWORD nValue;
	struct {
		DWORD oBufferStage1Vacuum : 1;		// Y0900
		DWORD o0901 : 1;					// Y0901
		DWORD o0902 : 1;					// Y0902
		DWORD oBufferStage1Rotate : 1;		// Y0903
		DWORD oBufferStage1Up : 1;			// Y0904
		DWORD oBufferStage1Down : 1;		// Y0905
		DWORD o0906 : 1;					// Y0906
		DWORD o0907 : 1;					// Y0907
		DWORD oBufferStage2Vacuum : 1;		// Y0908
		DWORD o0909 : 1;					// Y0909
		DWORD o0910 : 1;					// Y0910
		DWORD oBufferStage2Rotate : 1;		// Y0911
		DWORD oBufferStage2Up : 1;			// Y0912
		DWORD oBufferStage2Down : 1;		// Y0913
		DWORD o0914 : 1;					// Y0914
		DWORD o0915 : 1;					// Y0915
		DWORD o0916 : 1;					// Y0916
		DWORD o0917 : 1;					// Y0917
		DWORD o0918 : 1;					// Y0918
		DWORD o0919 : 1;					// Y0919
		DWORD o0920 : 1;					// Y0920
		DWORD o0921 : 1;					// Y0921
		DWORD o0922 : 1;					// Y0922
		DWORD o0923 : 1;					// Y0923
		DWORD o0924 : 1;					// Y0924
		DWORD o0925 : 1;					// Y0925
		DWORD oNgPortSlideLock : 1;			// Y0926
		DWORD oNgPortSlideUnlock : 1;		// Y0927
		DWORD o0928 : 1;					// Y0928
		DWORD o0929 : 1;					// Y0929
		DWORD o0930 : 1;					// Y0930
		DWORD o0931 : 1;					// Y0931
	};
} DY_DATA_09;		// Digital Output 09

typedef union tag_DY_DATA_10 {
	DWORD nValue;
	struct {
		DWORD o1000 : 1;					// Y1000
		DWORD oSortPicker1Down1 : 1;		// Y1001
		DWORD o1002 : 1;					// Y1002
		DWORD oSortPicker1Down2 : 1;		// Y1003
		DWORD o1004 : 1;					// Y1004
		DWORD oSortPicker1Down3 : 1;		// Y1005
		DWORD o1006 : 1;					// Y1006
		DWORD oSortPicker1Down4 : 1;		// Y1007
		DWORD o1008 : 1;					// Y1008
		DWORD oSortPicker1Down5 : 1;		// Y1009
		DWORD o1010 : 1;					// Y1010
		DWORD o1011 : 1;					// Y1011
		DWORD o1012 : 1;					// Y1012
		DWORD o1013 : 1;					// Y1013
		DWORD o1014 : 1;					// Y1014
		DWORD o1015 : 1;					// Y1015
		DWORD oSortPicker1Vac1 : 1;			// Y1016
		DWORD oSortPicker1Air1 : 1;			// Y1017
		DWORD oSortPicker1Vac2 : 1;			// Y1018
		DWORD oSortPicker1Air2 : 1;			// Y1019
		DWORD oSortPicker1Vac3 : 1;			// Y1020
		DWORD oSortPicker1Air3 : 1;			// Y1021
		DWORD oSortPicker1Vac4 : 1;			// Y1022
		DWORD oSortPicker1Air4 : 1;			// Y1023
		DWORD oSortPicker1Vac5 : 1;			// Y1024
		DWORD oSortPicker1Air5 : 1;			// Y1025
		DWORD o1026 : 1;					// Y1026
		DWORD oNgBufferVac01 : 1;			// Y1027
		DWORD oNgBufferVac02 : 1;			// Y1028
		DWORD oNgBufferVac03 : 1;			// Y1029
		DWORD oNgBufferVac04 : 1;			// Y1030
		DWORD oNgBufferVac05 : 1;			// Y1031
	};
} DY_DATA_10;		// Digital Output 10

typedef union tag_DY_DATA_11 {
	DWORD nValue;
	struct {
		DWORD o1100 : 1;					// Y1100
		DWORD oSortPicker2Down1 : 1;		// Y1101
		DWORD o1102 : 1;					// Y1102
		DWORD oSortPicker2Down2 : 1;		// Y1103
		DWORD o1104 : 1;					// Y1104
		DWORD oSortPicker2Down3 : 1;		// Y1105
		DWORD o1106 : 1;					// Y1106
		DWORD oSortPicker2Down4 : 1;		// Y1107
		DWORD o1108 : 1;					// Y1108
		DWORD oSortPicker2Down5 : 1;		// Y1109
		DWORD o1110 : 1;					// Y1110
		DWORD o1111 : 1;					// Y1111
		DWORD o1112 : 1;					// Y1112
		DWORD o1113 : 1;					// Y1113
		DWORD o1114 : 1;					// Y1114
		DWORD o1115 : 1;					// Y1115
		DWORD oSortPicker2Vac1 : 1;			// Y1116
		DWORD oSortPicker2Air1 : 1;			// Y1117
		DWORD oSortPicker2Vac2 : 1;			// Y1118
		DWORD oSortPicker2Air2 : 1;			// Y1119
		DWORD oSortPicker2Vac3 : 1;			// Y1120
		DWORD oSortPicker2Air3 : 1;			// Y1121
		DWORD oSortPicker2Vac4 : 1;			// Y1122
		DWORD oSortPicker2Air4 : 1;			// Y1123
		DWORD oSortPicker2Vac5 : 1;			// Y1124
		DWORD oSortPicker2Air5 : 1;			// Y1125
		DWORD o1126 : 1;					// Y1126
		DWORD oNgBufferVac06 : 1;			// Y1127
		DWORD oNgBufferVac07 : 1;			// Y1128
		DWORD oNgBufferVac08 : 1;			// Y1129
		DWORD oNgBufferVac09 : 1;			// Y1130
		DWORD oNgBufferVac10 : 1;			// Y1131
	};
} DY_DATA_11;	// Digital Output 11

typedef union tag_DY_DATA_12 {
	DWORD nValue;
	struct {
		DWORD o1200 : 1;					// Y1200
		DWORD o1201 : 1;					// Y1201
		DWORD o1202 : 1;					// Y1202
		DWORD o1203 : 1;					// Y1203
		DWORD o1204 : 1;					// Y1204
		DWORD o1205 : 1;					// Y1205
		DWORD o1206 : 1;					// Y1206
		DWORD o1207 : 1;					// Y1207
		DWORD oGoodPortSlideLock : 1;		// Y1208
		DWORD oGoodPortSlideUnlock : 1;		// Y1209
		DWORD o1210 : 1;					// Y1210
		DWORD o1211 : 1;					// Y1211
		DWORD oGoodTrayBuffSupport1In : 1;	// Y1212
		DWORD o1213 : 1;					// Y1213
		DWORD oGoodTrayBuffSupport2In : 1;	// Y1214
		DWORD o1215 : 1;					// Y1215
		DWORD o1216 : 1;					// Y1216
		DWORD o1217 : 1;					// Y1217
		DWORD o1218 : 1;					// Y1218
		DWORD o1219 : 1;					// Y1219
		DWORD oGoodStage1MasterIn : 1;		// Y1220
		DWORD o1221 : 1;					// Y1221
		DWORD oGoodStage1SlaveIn : 1;		// Y1222
		DWORD o1223 : 1;					// Y1223
		DWORD o1224 : 1;					// Y1224
		DWORD o1225 : 1;					// Y1225
		DWORD o1226 : 1;					// Y1226
		DWORD o1227 : 1;					// Y1227
		DWORD oGoodStage2MasterIn : 1;		// Y1228
		DWORD o1229 : 1;					// Y1229
		DWORD oGoodStage2SlaveIn : 1;		// Y1230
		DWORD o1231 : 1;					// Y1231
	};
} DY_DATA_12;	// Digital Output 12

typedef union tag_DY_DATA_13 {
	DWORD nValue;
	struct {
		DWORD o1300 : 1;					// Y1300
		DWORD o1301 : 1;					// Y1301
		DWORD o1302 : 1;					// Y1302
		DWORD o1303 : 1;					// Y1303
		DWORD o1304 : 1;					// Y1304
		DWORD o1305 : 1;					// Y1305
		DWORD o1306 : 1;					// Y1306
		DWORD o1307 : 1;					// Y1307
		DWORD oEmptyTrans1Up : 1;			// Y1308
		DWORD oEmptyTrans1Down : 1;			// Y1309
		DWORD oEmptyTrans1MasterIn : 1;		// Y1310
		DWORD oEmptyTrans1MasterOut : 1;	// Y1311
		DWORD oEmptyTrans1SlaveIn : 1;		// Y1312
		DWORD oEmptyTrans1SlaveOut : 1;		// Y1313
		DWORD o1314 : 1;					// Y1314
		DWORD o1315 : 1;					// Y1315
		DWORD o1316 : 1;					// Y1316
		DWORD o1317 : 1;					// Y1317
		DWORD o1318 : 1;					// Y1318
		DWORD o1319 : 1;					// Y1319
		DWORD o1320 : 1;					// Y1320
		DWORD o1321 : 1;					// Y1321
		DWORD o1322 : 1;					// Y1322
		DWORD o1323 : 1;					// Y1323
		DWORD oEmptyTrans2Up : 1;			// Y1324
		DWORD oEmptyTrans2Down : 1;			// Y1325
		DWORD oEmptyTrans2MasterIn : 1;		// Y1326
		DWORD oEmptyTrans2MasterOut : 1;	// Y1327
		DWORD oEmptyTrans2SlaveIn : 1;		// Y1328
		DWORD oEmptyTrans2SlaveOut : 1;		// Y1329
		DWORD o1330 : 1;					// Y1330
		DWORD o1331 : 1;					// Y1331
	};
} DY_DATA_13;	// Digital Output 13

typedef union tag_DY_DATA_14 {
	DWORD nValue;
	struct {
		DWORD oTowerGreen : 1;				// Y1400
		DWORD oTowerYellow : 1;				// Y1401
		DWORD oTowerRed : 1;				// Y1402
		DWORD oBuzzerBit0 : 1;				// Y1403
		DWORD oBuzzerBit1 : 1;				// Y1404
		DWORD oBuzzerBit2 : 1;				// Y1405
		DWORD oBuzzerBit3 : 1;				// Y1406
		DWORD oBuzzerBit4 : 1;				// Y1407
		DWORD o1408 : 1;					// Y1408
		DWORD o1409 : 1;					// Y1409
		DWORD o1410 : 1;					// Y1410
		DWORD o1411 : 1;					// Y1411
		DWORD oStartLamp1 : 1;				// Y1412
		DWORD oStartLamp2 : 1;				// Y1413
		DWORD oStartLamp3  : 1;				// Y1414
		DWORD oStopLamp1 : 1;				// Y1415
		DWORD oStopLamp2 : 1;				// Y1416
		DWORD oStopLamp3 : 1;				// Y1417
		DWORD oResetLamp1 : 1;				// Y1418
		DWORD oResetLamp2 : 1;				// Y1419
		DWORD oResetLamp3 : 1;				// Y1420
		DWORD o1421 : 1;					// Y1421
		DWORD o1422 : 1;					// Y1422
		DWORD o1423 : 1;					// Y1423
		DWORD oLoad1Lamp : 1;				// Y1424
		DWORD oLoad2Lamp  : 1;				// Y1425
		DWORD oNgLamp : 1;					// Y1426
		DWORD oGoodLamp : 1;				// Y1427
		DWORD oEmptyLamp : 1;				// Y1428
		DWORD o1429 : 1;					// Y1429
		DWORD o1430 : 1;					// Y1430
		DWORD o1431 : 1;					// Y1431
	};
} DY_DATA_14;	// Digital Output 14

typedef union tag_DY_DATA_15 {
	DWORD nValue;
	struct {
		DWORD oDoor01Unlock : 1;			// Y1500
		DWORD oDoor02Unlock : 1;			// Y1501
		DWORD oDoor03Unlock : 1;			// Y1502
		DWORD oDoor04Unlock : 1;			// Y1503
		DWORD oDoor05Unlock : 1;			// Y1504
		DWORD oDoor06Unlock : 1;			// Y1505
		DWORD oDoor07Unlock : 1;			// Y1506
		DWORD oDoor08Unlock : 1;			// Y1507
		DWORD oDoor09Unlock : 1;			// Y1508
		DWORD oDoor10Unlock : 1;			// Y1509
		DWORD oDoor11Unlock : 1;			// Y1510
		DWORD oDoor12Unlock : 1;			// Y1511
		DWORD oDoor13Unlock : 1;			// Y1512
		DWORD oDoor14Unlock : 1;			// Y1513
		DWORD oDoor15Unlock : 1;			// Y1514
		DWORD oDoor16Unlock : 1;			// Y1515
		DWORD oDoor17Unlock : 1;			// Y1516
		DWORD oDoor18Unlock : 1;			// Y1517
		DWORD oDoor19Unlock : 1;			// Y1518
		DWORD o1519 : 1;					// Y1519
		DWORD o1520 : 1;					// Y1520
		DWORD o1521 : 1;					// Y1521
		DWORD o1522 : 1;					// Y1522
		DWORD o1523 : 1;					// Y1523
		DWORD oInsideLight : 1;				// Y1524
		DWORD oSafetyReset : 1;				// Y1525
		DWORD oModeSelect  : 1;				// Y1526
		DWORD o1527 : 1;					// Y1527
		DWORD o1528 : 1;					// Y1528
		DWORD o1529 : 1;					// Y1529
		DWORD o1530 : 1;					// Y1530
		DWORD o1531 : 1;					// Y1531
	};
} DY_DATA_15;	// Digital Output 15

#endif
///////////////////////////////////////////////////////////////////////////////
