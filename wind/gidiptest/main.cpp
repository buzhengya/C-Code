//http request
//POST /v1?idip_sig=8f4d241be48e305237a9bd531ba3818d HTTP/1.1
//Content-Type:text/html
//  
//data_packet={}
#include "http_net.h"
#include "http_proto.h"
#include "gtest/gtest.h"
#include "proto1.h"
#include "wtime.h"
#define INNER_IP "192.168.3.70"
#define OUTER_IP "193.112.62.41"
#define JIAN_RONG "118.89.17.44"
#define PRESSURE_TEST "192.168.24.55"
int main(int argc, char* argv[])
{
	if (!CHttpNet::Instance()->Init(PRESSURE_TEST, 6787, 5000))
	{
		return 0;
	}

	if (!CHttpProto::Instance()->Init(INNER_IP))
	{
		return 0;
	}

	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}