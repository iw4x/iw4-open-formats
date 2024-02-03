#pragma once

namespace iw4of
{
	struct iw4_native_asset
	{
		int type;
		void* data;

		bool operator==(const iw4_native_asset& other) const{
			return data == other.data;
		};
		
		struct hash {
			size_t operator()(const iw4_native_asset& k) const {
				return static_cast<size_t>(k.type) ^ reinterpret_cast<size_t>(k.data);
			}
		};
	};
}
