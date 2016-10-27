// Charles Zheng
// charlesz
// CS3650
// Homework 7

#include <stdio.h>

struct cacheLine {
  int valid;
  int tag;
  int time;
};

struct cacheLine cache[10000];
// Initialize all valid fields to 0 (false)

int cache_size;
int block_size;
int num_sets;
int cache_lines;
int count;

int test_set[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64,
 68, 72, 76, 80, 0, 4, 8, 12, 16, 71, 3, 41, 81, 39, 38, 71, 15, 39, 11, 51, 57, 41};

int run();
int isHit(int tag, int set);
int evict(int tag, int set);
int toEvict(int set);
int printCache();

int main() {

	printf("Enter cache total size (in bytes): ");
	scanf("%d", &cache_size);

	printf("Enter block size (in bytes): ");
	scanf("%d", &block_size);

	printf("Enter number of sets: ");
	scanf("%d", &num_sets);

	cache_lines = cache_size / block_size;
	count = 0;

	for (int i = 0; i < cache_lines; i++) {
		cache[i].valid = 0;
		cache[i].tag = -1;
		cache[i].time = 0;
	}

	run();

	return 0;
}

int run() {
	for (int i = 0; i < sizeof(test_set) / sizeof(test_set[0]); i++) {
		int addr = test_set[i];
		int offset = addr % block_size;
		int set = (addr / block_size) % num_sets;
		int tag = addr / (cache_size / num_sets);
		count++;

		printf("%4d: ", addr);
		if (isHit(tag, set)) {
			printf("HIT  ");
		} else {
			printf("MISS ");
		}
		printf("(%d/%d/%d)\n", tag, set, offset);

	}

	printCache();

	return 0;
}

int isHit(int tag, int set) {
	for (int i = set * num_sets; i < (set + 1) * num_sets; i++) {
		struct cacheLine *cur = &cache[i];
		if(cur->valid == 1 && cur->tag == tag) {
			return 1;
		}
	}
	cache[toEvict(set)].valid = 1;
	cache[toEvict(set)].tag = tag;
	cache[toEvict(set)].time = count;
	count++;
	return 0;
}

int toEvict(int set) {
	int ans = set * num_sets;
	for (int i = set * num_sets; i < (set+1) * num_sets; i++) {
		if (cache[i].time < cache[ans].time) {
			ans = i;
		}
	}
	return ans;
}

int printCache() {
	printf("Cache contents: \n");
	for (int i = 0; i < cache_lines; i++) {
		struct cacheLine *cur = &cache[i];
		if (cur->tag >= 0) {
			printf("%4d: Valid: %d;  Tag:%4d  (Set #:%3d)\n", i, 
				cur->valid, cur->tag, i / num_sets);
		} else {
			printf("%4d: Valid: %d;  Tag:   -  (Set #:%3d)\n", i, 
				cur->valid, i / num_sets);
		}

	}
	return 0;
}



