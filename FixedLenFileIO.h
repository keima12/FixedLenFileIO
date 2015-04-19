#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//---------------------------
//固定長レコードアクセスクラス
//---------------------------
template<class T>
class ByteFileRecordIO
{
public:
	ByteFileRecordIO<T>(std::fstream &fs, unsigned int recordnum) : m_fs(fs), m_record_num(recordnum)
	{

	};
	T read()
	{
		T buf = { 0 };
		m_fs.seekg(m_record_num, std::ios::beg);
		m_fs.seekg(m_record_num*sizeof(T));
		m_fs.read((char *)&buf, sizeof(T));
		return buf;
	};
	void write(T &buf)
	{
		m_fs.seekg(0, std::ios::beg);
		m_fs.seekg(m_record_num*sizeof(T));
		m_fs.write((const char*)&buf, sizeof(T));
	};
	T operator=(T data)
	{
		this->write(data);
		return this->read();
	};

	operator T()
	{
		return this->read();
	}

private:
	std::fstream &m_fs;
	unsigned int m_record_num;
	
};


//----------------------------
// 固定長レコードファイルアクセスクラス
//-----------------------------
template<class T> class FixedLenFileIO
{
public:
	FixedLenFileIO() : fillename("") { };
	FixedLenFileIO(char* filename) : m_file_name(filename)
	{
		this->open(filename);
	};
	FixedLenFileIO(std::string filename) : filename(filename)
	{
		this->open(filename);
	};
	~FixedLenFileIO()
	{
		for (auto i = m_record_io.begin(); i != m_record_io.end(); ++i)
		{
			delete *i;
		}
	};
	
	int size()
	{
		m_record_io.size();
	}
	T read(unsigned int recordnum)
	{
		T buf;
		buf = m_record_io[recordnum]->read();
		return buf;
	};
	void write(unsigned int recordnum, const T& data)
	{
		m_record_io[recordnum]->write(data);
	};

	ByteFileRecordIO<T> & operator[](int recordnum)
	{
		return (*m_record_io[recordnum]);
	}

private:
	std::string  m_file_name;
	std::fstream m_fs;
	unsigned int m_record_num;
	std::vector<ByteFileRecordIO<T>* > m_record_io;

	void open(char *filename)
	{
		m_fs.seekg(0, std::ios::beg);
		m_fs.open(filename, std::ios::binary | std::ios::in | std::ios::out);
		int w = m_fs.tellg();
		m_fs.seekg(0, std::ios::end);
		int w2 = m_fs.tellg();
		m_fs.clear();
		m_record_num = (w2 - w)/sizeof(T);
		set_record_io();
	}

	void open(std::string filename)
	{
		this->open(filename.c_str())
	}
	void set_record_io()
	{
		for (int i = 0; i != m_record_num; ++i)
		{
			ByteFileRecordIO<T> *w = new ByteFileRecordIO<T>(m_fs, i);
			m_record_io.push_back(w);
		}
	};
};



