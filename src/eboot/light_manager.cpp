#include "light_manager.hpp"

#include "camera.hpp"
#include "vfpu.h"
#include "stage_manager.hpp"
#include "immediate_ge.hpp"

using namespace immediate_ge;

template<> LightManager *Singleton<LightManager>::objectPtr;

LightManager::LightManager() {
    memset(&data, 0, sizeof(data));
    unknown_0x15C = -1;
}

LightManager::~LightManager() {
    // empty
}

void LightManager::method_088607F0() {
    for (int i = 0; i < 2; ++i) {
        lighting_data *lighting = &data.lighting[i];
        if (i == 1U) {
            copy_q(&lighting->lights[2].position, &Camera::objectPtr->position);
        }
    }
}

ScePspFVector3 D_eboot_089A2C30[3] = {
    {-1, -1, -1},
    {1, 1, 1},
    {0, 0, 0}
};

ScePspFVector4Unaligned D_eboot_089A2C54[3] = {
    {0.4, 0.4, 0.4, 0},
    {0.4, 0.4, 0.4, 0},
    {0.5, 0.5, 0.5, 0}
};

ScePspFVector4Unaligned D_eboot_089A2C84[3] = {
    {0.3, 0.3, 0.3, 0},
    {0.3, 0.3, 0.3, 0},
    {0.3, 0.3, 0.3, 0}
};

void LightManager::method_08860840(light_data *light, s16 index, u16 operation) {
    if (unknown_0x15C != 0) {
        StageBase *stage = StageManager::objectPtr->stage;
        switch (operation) {
        case 0:
            light->position.x = -1.0f * stage->lights[index].position.x;
            light->position.y = -1.0f * stage->lights[index].position.y;
            light->position.z = -1.0f * stage->lights[index].position.z;
            light->ambient.x = stage->lights[index].ambient_color.x;
            light->ambient.y = stage->lights[index].ambient_color.y;
            light->ambient.z = stage->lights[index].ambient_color.z;
            // fallthrough
        case 1:
            light->diffuse.x = stage->lights[index].diffuse_color.x;
            light->diffuse.y = stage->lights[index].diffuse_color.y;
            light->diffuse.z = stage->lights[index].diffuse_color.z;
            break;
        default:
            break;
        }
    } else {
        switch (operation) {
        case 0:
            light->position.x = -1.0f * D_eboot_089A2C30[index].x;
            light->position.y = -1.0f * D_eboot_089A2C30[index].y;
            light->position.z = -1.0f * D_eboot_089A2C30[index].z;
            light->ambient.x = D_eboot_089A2C84[index].x;
            light->ambient.y = D_eboot_089A2C84[index].y;
            light->ambient.z = D_eboot_089A2C84[index].z;
            // fallthrough
        case 1:
            light->diffuse.x = D_eboot_089A2C54[index].x;
            light->diffuse.y = D_eboot_089A2C54[index].y;
            light->diffuse.z = D_eboot_089A2C54[index].z;
            break;
        default:
            break;
        }
    }
}

void LightManager::method_088609FC(s16 index) {
    memset(&data, 0, sizeof(data));
    unknown_0x15C = index;

    // TODO: refer to pmo implementation
    int offset = (int)&data.unknown_0x0 + sizeof(lighting_data);
    lighting_data *lighting = (lighting_data*)(offset);
    for (int i = 0; i < 3; ++i) {
        light_data (&lights)[3] = lighting->lights;
        u8 *light = (u8*)&lights[i];
        method_08860840((light_data *)(light + 0x10), i, 0);
    }
}

void LightManager::method_08860A70(ObjBase *c) {
    lighting_data *lighting = &data.lighting[1];
    for (int i = 0; i < 3; ++i) {
        light_data *light = &lighting->lights[i];
        if (c->diffuse_light_color_override == NULL || i == 2) {
            method_08860840(light, i, 1);
        } else {
            ScePspFVector4 &color = (*c->diffuse_light_color_override)[i];
            light->diffuse.x = color.x;
            light->diffuse.y = color.y;
            light->diffuse.z = color.z;
        }
    }
}

ScePspFVector4Unaligned D_eboot_089A2CB4[3] = {
    {0.1, 0.1, 0.2, 0},
    {0.1, 0.1, 0.2, 0},
    {0.1, 0.1, 0.2, 0}
};

void LightManager::method_08860B1C(Player *p) {
    lighting_data *lighting = &data.lighting[1];
    for (int i = 0; i < 3; ++i) {
        light_data *light = &lighting->lights[i];
        switch (unknown_0x15C) {
        case 0xC:
        case 0xE:
        case 0x1C:
        case 0x1E:
            if ((bool)(p->lighting_flags & 0x4000) == true) {
                light->diffuse.x = D_eboot_089A2CB4[i].x;
                light->diffuse.y = D_eboot_089A2CB4[i].y;
                light->diffuse.z = D_eboot_089A2CB4[i].z;
                continue;
            }
        default:
            break;
        }

        if (p->diffuse_light_color_override == NULL || i == 2) {
            method_08860840(light, i, 1);
        } else {
            ScePspFVector4 &color = (*p->diffuse_light_color_override)[i];
            light->diffuse.x = color.x;
            light->diffuse.y = color.y;
            light->diffuse.z = color.z;
        }
    }
}

void LightManager::method_08860C4C() {
    lighting_data *lighting = &data.lighting[1];
    for (int i = 0; i < 3; ++i) {
        method_08860840(&lighting->lights[i], i, 1);
    }
}

ScePspUnion32 LightManager::method_08860CA8(ObjBase *c, int i) {
    float b, g, r;
    lighting_data *lighting = &data.lighting[1];
    light_data *light = &lighting->lights[i];
    if (!c->diffuse_light_colors[i].ui) {
        r = light->diffuse.x;
        g = light->diffuse.y;
        b = light->diffuse.z;
    } else {
        r = c->diffuse_light_colors[i].uc[0] / 255.0f;
        g = c->diffuse_light_colors[i].uc[1] / 255.0f;
        b = c->diffuse_light_colors[i].uc[2] / 255.0f;
        r = r + (light->diffuse.x - r) / 5.0f;
        g = g + (light->diffuse.y - g) / 5.0f;
        b = b + (light->diffuse.z - b) / 5.0f;
    }
    ScePspUnion32 color;
    color.uc[0] = 255.0f * r;
    color.uc[1] = 255.0f * g;
    color.uc[2] = 255.0f * b;
    color.uc[3] = 0xFF;
    c->diffuse_light_colors[i].ui = color.ui;
    return color;
}

void LightManager::method_08860EB8(ObjBase *c) {
    lighting_data &lighting = data.lighting[1];
    for (int i = 0; i < 3; ++i) {
        light_data &light = lighting.lights[i];

        ScePspUnion32 color = method_08860CA8(c, i);

        ge::lighttype(i, GE_LIGHTTYPE_DIRECTIONAL);

        ScePspVector4 position;
        vtfm3_q(&position.fv, &Camera::objectPtr->world, &light.position);
        ge::lightposition(i, &position);

        ge::lightdiffusecolor(i, color.ui);

        color.uc[0] = 255.0f * light.ambient.x;
        color.uc[1] = 255.0f * light.ambient.y;
        color.uc[2] = 255.0f * light.ambient.z;
        ge::lightambientcolor(i, color.ui);
    }

    ge::lightingenable(true);
}

void LightManager::method_0886117C(s8 flag) {
    lighting_data &lighting = data.lighting[flag];
    for (int i = 0; i < 3; ++i) {
        light_data &light = lighting.lights[i];
        ScePspVector4 position;
        vtfm3_q(&position.fv, &Camera::objectPtr->world, &light.position);

        ge::lightposition(i, &position);
        ge::lighttype(i, GE_LIGHTTYPE_DIRECTIONAL);

        ScePspUnion32 color;
        color.uc[0] = 255.0f * light.diffuse.x;
        color.uc[1] = 255.0f * light.diffuse.y;
        color.uc[2] = 255.0f * light.diffuse.z;
        ge::lightdiffusecolor(i, color.ui);

        color.uc[0] = 255.0f * light.ambient.x;
        color.uc[1] = 255.0f * light.ambient.y;
        color.uc[2] = 255.0f * light.ambient.z;
        ge::lightambientcolor(i, color.ui);
    }

    ge::ambientcolor(0, 0, 0);
    ge::lightingenable(false);
}
