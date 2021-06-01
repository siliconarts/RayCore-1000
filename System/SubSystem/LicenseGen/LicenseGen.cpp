#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc==2 && !strcmp(argv[1], "test"))
	{
		g_LicenseManager.RunModal();
		return 0;
	}
	if(argc!=3){
		printf("LicenseGen.exe card_count request_code\n or \nLicenseGen.exe test\n");
		return 0;
	}

	{
		BYTE license_code[16];
		if(!g_LicenseManager.GenLicense(argv[2], license_code, atoi(argv[1]))){
			printf("Invalid request code string...\n");
			return 0;
		}
		{
			printf("Your license code : ");
			for(int i=0;i<16;i++)
				printf("%02X", license_code[i]);
		}
		{
			FILE* fp = fopen("license.txt", "w");

			if(!fp){
				printf("Can't create 'license.txt' file!!!\n");
				return 0;
			}

			fprintf(fp, "Your license code : ");
			for(int i=0;i<16;i++)
				fprintf(fp, "%02X", license_code[i]);

			printf("\n'license.txt' file is saved.\n");

			fclose(fp);
		}
	}
	
	return 0;
}
