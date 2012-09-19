/*
 *  File: atu_shm_buffer_example.cpp
 *  Description: An example of using atu_shm_buffer. Create and write the shared memory in
 *  one process, and read the shared memory in another process.
 *  Please add -lrt compile option.
 *  Created on: 2012-9-18
 *  Author: PArt
 *  Any questions, please mail me :) atu8292991@gmail.com
 */

#include <iostream>
#include "atu_shm_buffer.h"
using atu_libs::atu_shm_buffer::atu_shm_buffer;

#define SHM_BUFFER_LEN			128

int main(int argc, char *argv[])
{
	atu_shm_buffer shm_buf("shm_test", SHM_BUFFER_LEN, true);
	char buffer[SHM_BUFFER_LEN] = "";

	shm_buf.write("test", SHM_BUFFER_LEN);
	shm_buf.read((char *)buffer, SHM_BUFFER_LEN);

	std::cout << "Process id: " << getpid() << std::endl;
	std::cout << "Size: " << shm_buf.get_len() << std::endl;
	std::cout << "Write to " << shm_buf.get_name() << ": " << buffer << std::endl;

	if (0 == fork())
	{
		atu_shm_buffer shm_buf_client("shm_test");
		shm_buf_client.read((char *)buffer, SHM_BUFFER_LEN);
		std::cout << "Process id: " << getpid() << std::endl;
		std::cout << "Size: " << shm_buf.get_len() << std::endl;
		std::cout << "Read from "<< shm_buf_client.get_name() << ": " << buffer << std::endl;
	}

	getchar();
	return 0;
}
