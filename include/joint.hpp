#ifndef JOINT_HPP
#define JOINT_HPP

#include "common.h"

struct bind_pose {
    ScePspFMatrix4 transform;
    ScePspFVector3 scale;
    ScePspFVector3 rotation;
    ScePspFVector3 position;
    u32 unk_0x64;
    u16 f0x68[12];
};

struct JointFileBlock {
    enum { JOINT_FILE_LAST = 0x80000000u };

    u32 flags;
    u32 count;
    u32 size;

    int flGetHierarchyNum();
};

struct Joint {
    Joint();

    virtual ~Joint();
    virtual void update(ScePspFMatrix4 *, ScePspFMatrix4 *, float x, float y, float z);

    ScePspFVector3 f0x4;
    ScePspFMatrix4 f0x10;
    ScePspFMatrix4 f0x50;
    ScePspFMatrix4 localPose; // these might be swapped
    ScePspFMatrix4 globalPose;
    u16 f0x110;
    u16 f0x112;
    u16 key;
    u16 f0x116;
    float f0x118;
    float f0x11c;
    ScePspFMatrix3 f0x120;
    Joint *parent;
    Joint *sibling;
    Joint *child;
    bind_pose bind;
    bind_pose alt_bind;

    Joint *func_eboot_0885F920(Joint *arg1, u32 arg2);
    void func_eboot_0885FFB4(ScePspFVector4 *arg1, s32 arg2, float arg3, float arg4);

    void flGetHierarchyData2(void *block);
    void flInitPostureHierarchySISub(ScePspFMatrix4 *matrix);

    void setChild(Joint *child);
    void setParent(Joint *parent);
    void setSibling(Joint *sibling);

    void *jointFileNextBlock(JointFileBlock *block);
    void *jointFileNextJoint(JointFileBlock *block);
    u8 *jointFileData(JointFileBlock *block);

    static void operator delete(void *);
    static void *operator new(u32, void *);
};

struct JointFileData {
    u32 unknown_0x0;
    u32 root_count;
    u32 size;
    u32 roots[1];
};

struct JointBlock {
    u32 unknown_0x0;
    u32 count;
    u32 size;
    u32 bone_id;
    s32 up_parent;
    s32 left_child;
    s32 right_sibling;
    u8 padding_0x1C[0x50 - 0x1C];
    u32 chain_id;
};

struct Hierarchy {
    Hierarchy() {
        for (int i = 0; i < 4; ++i) {
            roots[i] = 0;
        }
        for (int i = 0; i < 4; ++i) {
            unknown_0x128[i] = 0;
            unknown_0x130[i] = 0;
            unknown_0x138[i] = 0;
        }
        root_count = 0;
        chain_count = 0;
    }
    virtual ~Hierarchy() {}

    u8 unknown_0x4[0x10C];
    Joint *roots[4];
    u16 root_count;
    u16 chain_count;
    u32 joint_count;
    u8 unknown_0x128[4];
    u32 *motion_table;
    u16 unknown_0x130[4];
    u16 unknown_0x138[4];

    void method_088640F0(int, float, float, float);
    void method_088641B8();
    void method_088642F4();

    static void flmatInit(ScePspFMatrix4 *m);

    void flGetHierarchy3_sub(JointFileBlock *block, void *buffer, float scaleX, float scaleY, float scaleZ) {
        int count = block->flGetHierarchyNum();
        joint_count = count;

        for (int i = 0; i < count; ++i) {
            new ((u8 *)buffer + i * sizeof(Joint)) Joint;
        }

        JointFileData *data = (JointFileData *)((Joint *)buffer)->jointFileData(block);
        JointBlock *jointBlock =
            (JointBlock *)((Joint *)buffer)->jointFileNextJoint((JointFileBlock *)data);
        chain_count = 0;

        for (int i = 0; i < count; ++i) {
            Joint *joint = (Joint *)buffer + i;
            joint->flGetHierarchyData2(jointBlock);
            if (jointBlock->up_parent != -1) {
                joint->setParent((Joint *)((u8 *)buffer + jointBlock->up_parent * sizeof(Joint)));
            }
            if (jointBlock->left_child != -1) {
                joint->setChild((Joint *)((u8 *)buffer + jointBlock->left_child * sizeof(Joint)));
            }
            if (jointBlock->right_sibling != -1) {
                joint->setSibling((Joint *)((u8 *)buffer + jointBlock->right_sibling * sizeof(Joint)));
            }
            if (chain_count <= jointBlock->chain_id) {
                chain_count = jointBlock->chain_id + 1;
            }
            jointBlock = (JointBlock *)joint->jointFileNextBlock((JointFileBlock *)jointBlock);
        }

        u32 *rootIndices = (u32 *)((u8 *)data + sizeof(JointFileBlock));
        for (int i = 0; i < data->root_count; ++i) {
            this->roots[i] = (Joint *)((u8 *)buffer + rootIndices[i] * sizeof(Joint));
        }
        root_count = data->root_count;

        ScePspFMatrix4 matrix;
        Hierarchy::flmatInit(&matrix);
        matrix.x.x /= scaleX;
        matrix.y.y /= scaleY;
        matrix.z.z /= scaleZ;
        for (int i = 0; i < root_count; ++i) {
            this->roots[i]->flInitPostureHierarchySISub(&matrix);
        }
    }

    static int flGetHierarchy3(Hierarchy *self, void *buffer, JointFileBlock *block, float scaleX, float scaleY, float scaleZ);

};

#endif

// TODO figure out shape for weak scope
#ifdef JOINT_PLAYER_MANAGER_IMPL
inline int Hierarchy::flGetHierarchy3(Hierarchy *self, void *buffer, JointFileBlock *block, float scaleX, float scaleY, float scaleZ) {
    if (self != NULL) {
        self->method_088642F4();
        self->flGetHierarchy3_sub(block, buffer, scaleX, scaleY, scaleZ);
        self->method_088640F0(0, 1.0f, 1.0f, 1.0f);
        self->method_088641B8();
        return 1;
    }
    return 0;
}

inline void Hierarchy::flmatInit(ScePspFMatrix4 *m) {
    vmidt_q(m);
}

inline void *Joint::jointFileNextBlock(JointFileBlock *block) {
    return (u8 *)block + block->size;
}

inline void Joint::setSibling(Joint *sibling) {
    this->sibling = sibling;
}

inline void Joint::setChild(Joint *child) {
    this->child = child;
}

inline void Joint::setParent(Joint *parent) {
    this->parent = parent;
}

inline void *Joint::jointFileNextJoint(JointFileBlock *block) {
    return (u8 *)block + block->size;
}

inline u8 *Joint::jointFileData(JointFileBlock *block) {
    return (u8 *)block + sizeof(JointFileBlock);
}

inline Joint::Joint() {
    bind.unk_0x64 = 0;
    alt_bind.unk_0x64 = 0;
    child = NULL;
    sibling = NULL;
    parent = NULL;
}

inline int JointFileBlock::flGetHierarchyNum() {
    int count = this->count;
    if (this->flags & JOINT_FILE_LAST) {
        count -= 1;
    }
    return count;
}

#endif
