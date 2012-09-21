/*
 * File: atu_shm_buffer.cpp
 * Description: A shared memory buffer. Read/write a shared memory just like a normal buffer.
 * Use a semaphore with the same name to synchronize the shared memory buffer in multi-process.
 * Created on: 2012-9-18
 * Author: PArt
 * Any questions, please mail me:) atu8292991@gmail.com
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "atu_shm_buffer.h"

namespace atu_libs {

namespace atu_shm_buffer {
/*
 *  Constructor of atu_shm_buffer.
 *  @name:     the name of the shared memory.
 *  @len:      if first created, specify the length of the shared memroy.
 *  @create:   true create a new shared memory, false link to an exist shared memory.
 */
atu_shm_buffer::atu_shm_buffer(const char *name, size_t len, bool create)
: m_buf(NULL)
, m_len(len)
, m_name(name)
{
	assert(NULL != name);

	int ret(0);

	//Link to the shared memory.
	if (create)
	{
		m_fd = shm_open(name, O_RDWR | O_CREAT, 0644);

		//create a semaphore for w/r operation.
		m_sem = sem_open(name, O_CREAT, 0644, 1);
		if (SEM_FAILED == m_sem)
		{
			fprintf(stderr, "Failed to create semaphore %s!\n", name);
			return;
		}
		//set the size of the shared memory.
		ret = ftruncate(m_fd, len);
		if (-1 == ret)
		{
			fprintf(stderr, "Failed to re-size shared memory buffer %s!\n", name);
			return;
		}
	}
	else
	{
		m_fd = shm_open(name, O_RDWR, 0644);
		//set a sem for w/r operation.
		m_sem = sem_open(name, O_RDWR);
		if (SEM_FAILED == m_sem)
		{
			fprintf(stderr, "Failed to create semaphore %s!\n", name);
			return;
		}
	}
	if (-1 == m_fd)
	{
		fprintf(stderr, "Failed to create shared memory buffer %s!\n", name);
		return;
	}

	//get shared memory status.
	ret = fstat(m_fd, &m_stat);
	if (-1 == ret)
	{
		fprintf(stderr, "Failed to get status of shared memory!\n");
		return;
	}
	m_len = m_stat.st_size;

	//map the shared memory to buffer.
	m_buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
	if (MAP_FAILED == m_buf)
	{
		perror("Failed to map shared memory to process!");
		return;
	}

	//If the shared memory is created for the first time, initialize it.
	if (create)
	{
		memset(m_buf, 0, len);
	}
}

atu_shm_buffer::~atu_shm_buffer() {
	if (-1 != m_fd)
	{
		munmap(m_buf, m_len);
	}

	//!!!If you want to unlink semaphore and shared memory, please use function unlink().
}

/*
 *  Unlink the shared memory and semaphore from linux kernel.
 */
void atu_shm_buffer::unlink(void)
{
	if (MAP_FAILED != m_buf)
	{
		sem_unlink(m_name.c_str());
		shm_unlink(m_name.c_str());
	}
}

/*
 *  Read from shared memory. Use a semaphore with the same name to synchronize.
 *  @buf:    the point to a buffer, the buffer must be allocated.
 *  @len:    the length of bytes read from shared memory.
 */
void atu_shm_buffer::read(char *buffer, size_t len)
{
	if (len > m_len)
	{
		fprintf(stderr, "[read] Buffer is truncated! From %d to %d bytes.\n",
				(int)len, (int)m_len);
		len = m_len;
	}
	sem_wait(m_sem);
	memcpy((void *)buffer, m_buf, len);
	sem_post(m_sem);
}
/*
 *  Read from shared memory. Use a semaphore with the same name to synchronize.
 *  @str:    the reference of string to store the content.
 *  @len:    the length of bytes read from shared memory.
 */
void atu_shm_buffer::read(std::string &str, size_t len)
{
	if (len > m_len)
	{
		fprintf(stderr, "[read] Buffer is truncated! From %d to %d bytes.\n",
				(int)len, (int)m_len);
		len = m_len;
	}
	sem_wait(m_sem);
	str.assign((char *)m_buf, len);
	sem_post(m_sem);
}

/*
 *  Write bytes to shared memroy. Use a semaphore with the same name to synchronize.
 *  @content:  the point to a buffer with contents, the buffer must be allocated.
 *  @len:      the length of bytes write to shared memory.
 */
void atu_shm_buffer::write(const void* buffer, size_t len)
{
	if (len > m_len)
	{
		fprintf(stderr, "[write] Buffer is truncated! From %d to %d bytes.\n",
				(int)len, (int)m_len);
		len = m_len;
	}
	sem_wait(m_sem);
	memcpy(m_buf, buffer, len);
	sem_post(m_sem);
}

/*
 *  Write bytes to shared memroy. Use a semaphore with the same name to synchronize.
 *  @str:      the reference of string with contents to write to shared memory.
 *  @len:      the length of bytes write to shared memory.
 */
void atu_shm_buffer::write(std::string &str, size_t len)
{
	if (len > m_len)
	{
		fprintf(stderr, "[write] Buffer is truncated! From %d to %d bytes.\n",
				(int)len, (int)m_len);
		len = m_len;
	}
	sem_wait(m_sem);
	memcpy(m_buf, str.c_str(), len);
	sem_post(m_sem);
}

}
}

