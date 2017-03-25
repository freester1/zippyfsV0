#ifndef BPLUS_INDEX_H
#define BPLUS_INDEX_H
#include "includes.h"
#include "inode.h"
#include "tire_fire.h"
#include "block.h"
#define ORDER 1000
/**
 * This class represents a memory-backed B+Tree used for indexing
 * inodes flushed from the in-memory file-system.
 *  - It will only support the limited functionality required by the file system, which is only insertion.
 * It utilizes the memory allocated dubbed TireFire, which also is implemented using bump-allocation, and thus free-ing is not available, and so deletions are not possible.
 *
 * @author Arvin Sharma
 */
class BPLUSIndex {

  public:
    /** represents an inode which is easily flushable to disk from memory */
    typedef struct inode {

        // stat info
        uint32_t mode;
        uint32_t nlink;
        uint64_t mtime;
        uint64_t ctime;
        uint64_t size;
        int deleted;
        // index to hash memory, always 512 size char*
        int64_t hash;
        // index to block data list memory
        int64_t block_data;
        // total size of block data
        uint64_t block_data_size;
    } inode;

    /** represents [block id, data] for inodes */
    typedef struct block_data {
        uint64_t block_id;
        uint64_t mtime;
        uint64_t size;
        // index to data memory
        int64_t data_offset;
    } block_data;

    /** represents a tree structure containing inodes */
    typedef struct node {
        int64_t parent = -1;
        int64_t keys[ORDER - 1] = {-1};
        int64_t num_keys = 0;
        int is_leaf = 0;
        /** child nodes */
        int64_t children[ORDER] = {-1};
        /** list of inodes if this node is a leaf */
        int64_t inodes;
        int64_t values_size = 0;
    } node;

    /** adds the given inode to this B+Tree */
    void add_inode(Inode in, std::map<uint64_t, std::shared_ptr<Block>> dirty_blocks, std::map<uint64_t, unsigned long long> block_mtime);

    /** constructor and destructor */
    BPLUSIndex();
    ~BPLUSIndex();

  private:
    /** in memory structure */
    TireFire mem_;
    uint64_t rootidx_ = 0;
    /** maintain pointer to root */
    node* root_ptr_ = nullptr;
    bool root_has_inode_ = false;


};
#endif