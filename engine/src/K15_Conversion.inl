/**
 * @file K15_Conversion.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

template<typename T>
T Conversion::ByteToKilobyte(T byte)
{
	return (T)(byte / 1024);
}

template <typename T>
T Conversion::ByteToMegabyte(T byte)
{
	return (T)(ByteToKilobyte<T>(byte) / 1024);
}

template <typename T>
T Conversion::ByteToGigabyte(T byte)
{
	return (T)(ByteToKilobyte<T>(byte) / 1024);
}

template <typename T>
T Conversion::KilobyteToByte(size_t kilobyte)
{
	return (T)(kilobyte * 1024);
}

template <typename T>
T Conversion::KilobyteToMegabyte(size_t kilobyte)
{
	return (T)(kilobyte / 1024);
}

template <typename T>
T Conversion::KilobyteToGigabyte(size_t kilobyte)
{
	return (T)(KilobyteToMegabyte<T>(kilobyte) / 1024);
}

template <typename T>
T Conversion::MegabyteToByte(size_t megabyte)
{
	return (T)(MegabyteToKilobyte<T>(megabyte) * 1024);
}

template <typename T>
T Conversion::MegabyteToKilobyte(size_t megabyte)
{
	return (T)(megabyte * 1024);
}

template <typename T>
T Conversion:: MegabyteToGigabyte(size_t megabyte)
{
	return (T)(megabyte / 1024);
}

template <typename T>
T Conversion::GigabyteToByte(size_t gigabyte)
{
	return (T)(GigabyteToKilobyte<T>(gigabyte) * 1024);
}

template <typename T>
T Conversion::GigabyteToKilobyte(size_t gigabyte)
{
	return (T)(GigabyteToMegabyte<T>(gigabyte) * 1024);
}

template <typename T>
T Conversion::GigabyteToMegabyte(size_t gigabyte)
{
	return (T)(gigabyte * 1024);
}