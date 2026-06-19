#include "gtest/gtest.h"

#include "../src/LRUCache.h"
#include <thread>
#include <chrono>
#include "../src/PersistenceManager.h"
#include <cstdio>

TEST(LRUCacheTest, TTLExpiration)
{
    LRUCache cache(3);

    cache.set("otp", "123456", 1);

    EXPECT_EQ(cache.get("otp"), "123456");

    std::this_thread::sleep_for(
        std::chrono::seconds(2));

    EXPECT_EQ(cache.get("otp"), "NULL");
}
TEST(LRUCacheTest, BasicSetGet)
{
    LRUCache cache(3);

    cache.set("name", "Dazai");

    EXPECT_EQ(cache.get("name"), "Dazai");
}

TEST(LRUCacheTest, MissingKey)
{
    LRUCache cache(3);

    EXPECT_EQ(cache.get("unknown"), "NULL");
}

TEST(LRUCacheTest, DeleteKey)
{
    LRUCache cache(3);

    cache.set("name", "Dazai");

    EXPECT_TRUE(cache.erase("name"));

    EXPECT_EQ(cache.get("name"), "NULL");
}

TEST(LRUCacheTest, LRUEviction)
{
    LRUCache cache(3);

    cache.set("A", "1");
    cache.set("B", "2");
    cache.set("C", "3");

    cache.get("A");

    cache.set("D", "4");

    EXPECT_EQ(cache.get("A"), "1");
    EXPECT_EQ(cache.get("C"), "3");
    EXPECT_EQ(cache.get("D"), "4");

    EXPECT_EQ(cache.get("B"), "NULL");
}
TEST(LRUCacheTest, ExistsKey)
{
    LRUCache cache(3);

    cache.set("name", "Dazai");

    EXPECT_TRUE(cache.exists("name"));
    EXPECT_FALSE(cache.exists("unknown"));
}

TEST(LRUCacheTest, CacheSize)
{
    LRUCache cache(3);

    EXPECT_EQ(cache.size(), 0);

    cache.set("A", "1");
    cache.set("B", "2");

    EXPECT_EQ(cache.size(), 2);
}

TEST(LRUCacheTest, ClearCache)
{
    LRUCache cache(3);

    cache.set("A", "1");
    cache.set("B", "2");

    cache.clear();

    EXPECT_EQ(cache.size(), 0);
    EXPECT_EQ(cache.get("A"), "NULL");
    EXPECT_EQ(cache.get("B"), "NULL");
}

TEST(LRUCacheTest, TTLRemainingTime)
{
    LRUCache cache(3);

    cache.set("otp", "123456", 10);

    long long remaining = cache.ttl("otp");

    EXPECT_GT(remaining, 0);
    EXPECT_LE(remaining, 10);
}
TEST(PersistenceTest, SaveAndLoadCache)
{
    std::string testFileName = "../data/test_cache.dat";

    {
        LRUCache cache(3);

        cache.set("name", "Dazai");
        cache.set("city", "Mumbai");

        PersistenceManager persistenceManager(testFileName);

        persistenceManager.save(cache);
    }

    {
        LRUCache loadedCache(3);

        PersistenceManager persistenceManager(testFileName);

        persistenceManager.load(loadedCache);

        EXPECT_EQ(loadedCache.get("name"), "Dazai");
        EXPECT_EQ(loadedCache.get("city"), "Mumbai");
    }

    std::remove(testFileName.c_str());
}