#include "Item.h"
Item::Item(Texture2D* tex, Vector2 pos) {
    texture = tex;
    position = pos;
    active = true;
    width = texture->width;
    height = texture->height;
    radius = (width + height) / 4.0f;
}
void Item::draw() {
    DrawTexture(*texture, (int)position.x, (int)position.y, WHITE);
}
Vector2 Item::getCenter() const {
    return { position.x + width / 2.0f, position.y + height / 2.0f };
}