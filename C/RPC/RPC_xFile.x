struct MyStruct {
	int field1;
	int field2;
};

program MYPROG {
	version MYVERS {
		int P1(MyStruct) = 1;
		int P2(MyStruct) = 2;
	} = 1;
} = 0x20000013;