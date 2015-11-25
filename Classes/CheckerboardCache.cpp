#include "CheckerboardCache.h"

#include "cocos2d.h"
#include "json/document.h"
using namespace cocos2d;


CheckerboardCache::CheckerboardCache()
{

}

CheckerboardCache::~CheckerboardCache()
{

}

// 添加棋盘配置
void CheckerboardCache::add_checkerboard_config(const std::string &filename)
{
	auto itr = checkerboard_.find(filename);
	if (itr == checkerboard_.end())
	{
		Data data = FileUtils::getInstance()->getDataFromFile(filename);
		if (!data.isNull())
		{
			rapidjson::Document doc;
			doc.Parse<0>(std::string((const char *)data.getBytes(), data.getSize()).c_str());

			auto map_info = TMXMapInfo::create(filename);
			auto layers = map_info->getLayers();
			CCAssert(!layers.empty(), "");
			auto tiles = layers.front()->_tiles;
			auto layer_ize = layers.front()->_layerSize;

			Config config;
			config.width = layer_ize.width;
			config.height = layer_ize.height;
			config.type_num = atoi(layers.front()->_name.c_str());
			const size_t max_size = layer_ize.width * layer_ize.height;
			for (size_t idx = 0; idx < max_size; ++idx)
			{
				config.layout.push_back(tiles[idx] != 0);
			}
			checkerboard_.insert(std::make_pair(filename, config));
		}
	}
}

// 获取棋盘配置
bool CheckerboardCache::get_checkerboard_config(const std::string &filename, Config &ret) const
{
	auto itr = checkerboard_.find(filename);
	if (itr != checkerboard_.end())
	{
		ret = itr->second;
	}
	return itr == checkerboard_.end() ? false : true;
}