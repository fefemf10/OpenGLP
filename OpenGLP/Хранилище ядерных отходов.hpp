#pragma once
//while (true)
//{
//	if (sup[isup].z + sup[isup].w == 16) break;
//	bool f = false;
//	for (short x = sup[isup].x; x < sup[isup].x + sup[isup].y; ++x)
//	{
//		if (!cup[sup[isup].z + sup[isup].w][x] && isTB({ x, y + 1, sup[isup].z + sup[isup].w }) && blocks[y][sup[isup].z][sup[isup].x].getType() == blocks[y][sup[isup].z + sup[isup].w][x].getType())
//			continue;
//		else
//		{
//			f = true;
//			break;
//		}
//	}
//	if (f) break;
//	for (short x = sup[isup].x; x < sup[isup].x + sup[isup].y; ++x)
//		cup[sup[isup].z + sup[isup].w][x] = true;
//	++sup[isup].w;
//}
//while (true)
//{
//	if (sdown[isdown].z + sdown[isdown].w == 16) break;
//	bool f = false;
//	for (short x = sdown[isdown].x; x < sdown[isdown].x + sdown[isdown].y; ++x)
//	{
//		if (!cdown[sdown[isdown].z + sdown[isdown].w][x] && isTB({ x, y - 1, sdown[isdown].z + sdown[isdown].w }) && blocks[y][sdown[isdown].z][sdown[isdown].x].getType() == blocks[y][sdown[isdown].z + sdown[isdown].w][x].getType())
//			continue;
//		else
//		{
//			f = true;
//			break;
//		}
//	}
//	if (f) break;
//	for (short x = sdown[isdown].x; x < sdown[isdown].x + sdown[isdown].y; ++x)
//		cdown[sdown[isdown].z + sdown[isdown].w][x] = true;
//	++sdown[isdown].w;
//}
//while (true)
//{
//	if (sleft[isleft].z + sleft[isleft].w == maxHeightBlock + 1) break;
//	bool f = false;
//	for (short x = sleft[isleft].x; x < sleft[isleft].x + sleft[isleft].y; ++x)
//	{
//		if (!cleft[sleft[isleft].z + sleft[isleft].w][x] && isTB({ z - 1, sleft[isleft].z + sleft[isleft].w, x }) && blocks[sleft[isleft].z][sleft[isleft].x][z].getType() == blocks[sleft[isleft].z + sleft[isleft].w][x][z].getType())
//			continue;
//		else
//		{
//			f = true;
//			break;
//		}
//	}
//	if (f) break;
//	for (short x = sleft[isleft].x; x < sleft[isleft].x + sleft[isleft].y; ++x)
//		cleft[sleft[isleft].z + sleft[isleft].w][x] = true;
//	++sleft[isleft].w;
//}
//while (true)
//{
//	if (sright[isright].z + sright[isright].w == maxHeightBlock + 1) break;
//	bool f = false;
//	for (short x = sright[isright].x; x < sright[isright].x + sright[isright].y; ++x)
//	{
//		if (!cright[sright[isright].z + sright[isright].w][x] && isTB({ z + 1, sright[isright].z + sright[isright].w, x }) && blocks[sright[isright].z][sright[isright].x][z].getType() == blocks[sright[isright].z + sright[isright].w][x][z].getType())
//			continue;
//		else
//		{
//			f = true;
//			break;
//		}
//	}
//	if (f) break;
//	for (short x = sright[isright].x; x < sright[isright].x + sright[isright].y; ++x)
//		cright[sright[isright].z + sright[isright].w][x] = true;
//	++sright[isright].w;
//}
//while (true)
//{
//	if (sfront[isfront].z + sfront[isfront].w == maxHeightBlock + 1) break;
//	bool f = false;
//	for (short x = sfront[isfront].x; x < sfront[isfront].x + sfront[isfront].y; ++x)
//	{
//		if (!cfront[sfront[isfront].z + sfront[isfront].w][x] && isTB({ x, sfront[isfront].z + sfront[isfront].w, z + 1 }) && blocks[sfront[isfront].z][z][sfront[isfront].x].getType() == blocks[sfront[isfront].z + sfront[isfront].w][z][x].getType())
//			continue;
//		else
//		{
//			f = true;
//			break;
//		}
//	}
//	if (f) break;
//	for (short x = sfront[isfront].x; x < sfront[isfront].x + sfront[isfront].y; ++x)
//		cfront[sfront[isfront].z + sfront[isfront].w][x] = true;
//	++sfront[isfront].w;
//}
//while (true)
//{
//	if (sback[isback].z + sback[isback].w == maxHeightBlock + 1) break;
//	bool f = false;
//	for (short x = sback[isback].x; x < sback[isback].x + sback[isback].y; ++x)
//	{
//		if (!cback[sback[isback].z + sback[isback].w][x] && isTB({ x, sback[isback].z + sback[isback].w, z - 1 }) && blocks[sback[isback].z][z][sback[isback].x].getType() == blocks[sback[isback].z + sback[isback].w][z][x].getType())
//			continue;
//		else
//		{
//			f = true;
//			break;
//		}
//	}
//	if (f) break;
//	for (short x = sback[isback].x; x < sback[isback].x + sback[isback].y; ++x)
//		cback[sback[isback].z + sback[isback].w][x] = true;
//	++sback[isback].w;
//}
//for (short y = 0; y < maxHeightBlock + 1; ++y)
//{
//	bool cup[16][16]{};
//	bool pass = true;
//	int isup = -1;
//	unsigned short oldType;
//	std::vector<glm::u8vec4> sup;
//	std::vector<glm::u8vec4> sdown;
//	for (short z = 0; z < 16; ++z)
//	{
//		for (short x = 0; x < 16; ++x)
//		{
//			unsigned short type = blocks[y][z][x].getType();
//			bool isTBlock = isTB({ x, y + 1, z });
//			if (!cup[z][x] && type != Block::AIR && isTBlock)
//			{
//				if (pass)
//				{
//					sup.push_back({ x, 1, z, 1 });
//					pass = false;
//					++isup;
//					oldType = type;
//				}
//				else
//				{
//					if (oldType == type)
//					{
//						++sup[isup].y;
//					}
//					else
//					{
//						for (short z = sup[isup].z + 1; z < 16; ++z)
//						{
//							bool flag = true;
//							for (short x = sup[isup].x; x < sup[isup].x + sup[isup].y; ++x)
//							{
//								if (blocks[y][z][x].getType() != Block::AIR && oldType == blocks[y][z][x].getType() && isTB({ x, y + 1, z }))
//								{
//								}
//								else
//								{
//									flag = false;
//									break;
//								}
//							}
//							if (flag)
//							{
//								memset(&cup[z][sup[isup].x], true, sup[isup].y);
//								++sup[isup].w;
//							}
//						}
//						sup.push_back({ x, 1, z, 1 });
//						++isup;
//						oldType = type;
//					}
//				}
//				cup[z][x] = true;
//			}
//			else
//			{
//				if (isup != -1 && type != Block::AIR)
//				{
//					for (short z = sup[isup].z + 1; z < 16; ++z)
//					{
//						bool flag = true;
//						for (short x = sup[isup].x; x < sup[isup].x + sup[isup].y; ++x)
//						{
//							if (blocks[y][z][x].getType() != Block::AIR && oldType == blocks[y][z][x].getType() && isTB({ x, y + 1, z }))
//							{
//							}
//							else
//							{
//								flag = false;
//								break;
//							}
//						}
//						if (flag)
//						{
//							memset(&cup[z][sup[isup].x], true, sup[isup].y);
//							++sup[isup].w;
//						}
//					}
//					sup.push_back({ x, 1, z, 1 });
//					++isup;
//					oldType = type;
//				}
//			}
//		}
//	}