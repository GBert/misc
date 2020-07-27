/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2020 Dominik (Teddy) Mahrer - www.railcontrol.org

RailControl is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

RailControl is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RailControl; see the file LICENCE. If not see
<http://www.gnu.org/licenses/>.
*/

#include <string>

#include "Utils/Utils.h"
#include "zlib/zlib.h"
#include "ZLib.h"

using std::string;

string ZLib::Compress(const string& input)
{
	long inputSize = input.size();

	unsigned long outputSize = inputSize + 12;
	unsigned char* outputBuffer = reinterpret_cast<unsigned char*>(malloc(outputSize));

	int nResult = compress2(outputBuffer, &outputSize, reinterpret_cast<const unsigned char*>(input.c_str()), inputSize, 9);

	if (nResult != Z_OK)
	{
		return "";
	}

	string outputData(reinterpret_cast<char*>(outputBuffer), outputSize);
	free(outputBuffer);
	return outputData;
}

string ZLib::UnCompress(const char* input, const size_t inputSize, const size_t outputSize)
{
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	int ret = inflateInit(&strm);
	if (ret != Z_OK)
	{
		return "";
	}
	strm.avail_in = inputSize;
	strm.avail_out = outputSize;
	strm.next_in = reinterpret_cast<unsigned char*>(const_cast<char*>(input));
	char* outputBuffer = reinterpret_cast<char*>(malloc(outputSize));
	strm.next_out = reinterpret_cast<unsigned char*>(outputBuffer);
	ret = inflate(&strm, Z_NO_FLUSH);
	inflateEnd(&strm);
	if (ret != Z_STREAM_END)
	{
		free(outputBuffer);
		return "";
	}

	std::string output(outputBuffer, outputSize);
	free(outputBuffer);
	return output;
}
