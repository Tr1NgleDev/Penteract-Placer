#include "ui.h"
#include <cassert>

void ui::page::addElem(element* e)
{
	elems.push_back(e);

	if (window != nullptr)
	{
		e->init(this);
	}
}

bool ui::page::removeElem(element* e)
{
	for (auto it = elems.begin(); it != elems.end(); ++it)
	{
		if (*it == e)
		{
			// don't remove the element directly so it can be processed correctly
			removeElem(it);
			return true;
		}
	}

	return false;
}

void ui::page::removeElem(container_t::iterator it)
{
	// must get index from iterator before erasing
	int index = it - elems.begin();
	if (index == focusIndex)
	{
		focusIndex = -1;
	}
	else if (index < focusIndex)
	{
		--focusIndex;
	}

	if (index == mouseDownIndex)
	{
		mouseDownIndex = -1;
	}
	else if (index < mouseDownIndex)
	{
		--mouseDownIndex;
	}

	elems.erase(it);
}

void ui::page::clear()
{
	focusIndex = -1;
	mouseDownIndex = -1;
	elems.clear();
}

ui::page::container_t::iterator ui::page::begin()
{
	return elems.begin();
}

ui::page::container_t::iterator ui::page::end()
{
	return elems.end();
}

ui::page::container_t::const_iterator ui::page::begin() const
{
	return elems.begin();
}

ui::page::container_t::const_iterator ui::page::end() const
{
	return elems.end();
}

void ui::page::init(GLFWwindow* w)
{
	window = w;

	int wWidth, wHeight;
	glfwGetFramebufferSize(window, &wWidth, &wHeight);
	windowResize(wWidth, wHeight);

	for (auto elem : elems)
	{
		elem->init(this);
	}
}

void ui::page::render()
{
	for (auto elem : elems)
	{
		elem->render(this);
	}
}

void ui::page::mouseInput(double xpos, double ypos)
{
	for (auto elem : elems)
	{
		elem->render(this);
	}
}

bool ui::page::scrollInput(double xoff, double yoff)
{
	double xpos, ypos;
	int mx, my;
	glfwGetCursorPos(window, &xpos, &ypos);
	mx = xpos;
	my = ypos;

	for (auto elem : elems)
	{
		if (!elem->clickable())
		{
			continue;
		}

		int x, y, w, h;
		elem->getBounds(this, &x, &y, &w, &h);
		if (mx < x || mx < y || mx >= x + w || mx >= y + h)
		{
			continue;
		}

		if (elem->scrollInput(this, xoff, yoff))
		{
			return true;
		}
	}

	return false;
}

bool ui::page::mouseButtonInput(int button, int action, int mods)
{
	double xpos, ypos;
	int mx, my;
	glfwGetCursorPos(window, &xpos, &ypos);
	mx = xpos;
	my = ypos;

	for (auto it = elems.begin(); it != elems.end(); ++it)
	{
		element* elem = *it;
		if (!elem->clickable())
		{
			continue;
		}

		int x, y, w, h;
		elem->getBounds(this, &x, &y, &w, &h);
		if (mx < x || my < y || mx >= x + w || my >= y + h)
		{
			continue;
		}

		if (elem->mouseButtonInput(this, xpos, ypos, button, action, mods))
		{
			focusIndex = it - elems.begin();

			if (mouseDownIndex != focusIndex)
			{
				mouseDownCancel();
			}
			mouseDownIndex = focusIndex;
			return true;
		}
	}

	mouseDownCancel();
	return false;
}

bool ui::page::keyInput(int key, int scancode, int action, int mods)
{
	element* elem = getFocusedElem();
	if (elem == nullptr)
	{
		return false;
	}

	return elem->keyInput(this, key, scancode, action, mods);
}

void ui::page::windowResize(int width, int height)
{
	glm::mat4 projection2D = glm::ortho(
		0.0f, static_cast<float>(width),
		static_cast<float>(height), 0.0f, -1.0f, 1.0f
	);

	// update the projection matrices for all of the shaders
	Shader::get("quad")->setUniform("P", projection2D);
	Shader::get("text")->setUniform("P", projection2D);
	Shader::get("tex")->setUniform("proj", projection2D);

	for (auto elem : elems)
	{
		elem->windowResize(this, width, height);
	}
}

bool ui::page::charInput(uint32_t codepoint)
{
	element* elem = getFocusedElem();
	if (elem == nullptr)
	{
		return false;
	}

	return elem->charInput(this, codepoint);
}

bool ui::page::fileDrop(int count, const char* paths[])
{
	element* elem = getFocusedElem();
	if (elem == nullptr)
	{
		return false;
	}

	return elem->fileDrop(this, count, paths);
}

GLFWwindow* ui::page::getWindow()
{
	return window;
}

ui::element* ui::page::getFocusedElem()
{
	assert(focusIndex < 0 || focusIndex < elems.size());
	if (focusIndex > 0)
	{
		return elems[focusIndex];
	}
	return nullptr;
}

void ui::page::mouseDownCancel()
{
	if (mouseDownIndex < 0)
	{
		return;
	}

	assert(mouseDownIndex < elems.size());
	elems[mouseDownIndex]->mouseDownCancel();

	mouseDownIndex = -1;
}

QuadRenderer ui::element::qr;
TextRenderer ui::element::tr;
void ui::element::renderInit()
{
	qr.setShader(Shader::get("quad"));
	qr.init();

	Texture::load("assets/textures/pixelfont.png", 0, false, "pixelfont.png");
	tr.setShader(Shader::get("text"));
	tr.setTexture(Texture::get("pixelfont.png"));
	tr.init();
}

ui::alignx ui::element::getAlignX() const
{
	return alignmentX;
}

ui::aligny ui::element::getAlignY() const
{
	return alignmentY;
}

void ui::element::setAlignX(alignx alignment)
{
	alignmentX = alignment;
}

void ui::element::setAlignY(aligny alignment)
{
	alignmentY = alignment;
}

int ui::element::getOffsetX() const
{
	return offsetX;
}

int ui::element::getOffsetY() const
{
	return offsetY;
}

void ui::element::setOffsetX(int offset)
{
	offsetX = offset;
}

void ui::element::setOffsetY(int offset)
{
	offsetY = offset;
}

void ui::text::getBounds(page* p, int* x, int* y, int* w, int* h)
{
	getBoundSize(w, h);

	int wWidth, wHeight;
	glfwGetFramebufferSize(p->getWindow(), &wWidth, &wHeight);

	switch (alignmentX)
	{
	case ALIGN_LEFT:
		*x = 0 + offsetX;
		break;
	case ALIGN_RIGHT:
		*x = wWidth - *w + offsetX;
		break;
	case ALIGN_CENTER_X:
	default:
		*x = (wWidth / 2) - (*w / 2) + offsetX;
		break;
	}

	switch (alignmentY)
	{
	case ALIGN_TOP:
		*y = 0 + offsetY;
		break;
	case ALIGN_BOTTOM:
		*y = wHeight - *h + offsetY;
		break;
	case ALIGN_CENTER_Y:
	default:
		*y = (wHeight / 2) - (*h / 2) + offsetY;
		break;
	}
}

void ui::text::render(page* p)
{
	// copied from a really old version of 4dm (for the most part) lol
	int x, y, w, h;
	getBounds(p, &x, &y, &w, &h);

	int charWidth = tr.getCharWidth() * size;
	int charHeight = tr.getCharHeight() * size * 1.5;

	if (wrapWidth == 0)
	{
		size_t yOffset = 0;
		size_t lineStart = 0;
		size_t pos = 0;
		for (;;)
		{
			pos = glm::min(text.find('\n', pos), text.length());
			renderText(p, text.substr(lineStart, pos - lineStart), x, y + yOffset * charHeight, false);

			if (pos == text.length())
			{
				break;
			}

			lineStart = pos;
			++pos;
			++yOffset;
		}
		return;
	}

	if (alignmentX == ALIGN_CENTER_X)
	{
		x += wrapWidth / 2;
	}
	else if (alignmentX == ALIGN_RIGHT)
	{
		x += wrapWidth;
	}

	size_t yOffset = 0;
	size_t lineStart = 0;
	size_t lastWordEnd = 0;
	size_t wordEnd = 0;
	size_t pos = 0;
	while (pos + 1 < text.length())
	{
		pos = glm::min(text.find(' ', pos), text.find('\n', pos));

		if (pos == std::string::npos)
		{
			pos = text.length() - 1;
		}

		lastWordEnd = wordEnd;
		wordEnd = pos;
		++pos;

		if (text[lastWordEnd] == '\n' || (wordEnd - lineStart) * charWidth > wrapWidth)
		{
			size_t lineEnd = (lastWordEnd == lineStart ? wordEnd : lastWordEnd);
			renderText(p, text.substr(lineStart, lineEnd - lineStart), x, y + yOffset * charHeight, true);
			lineStart = lineEnd + 1;
			lastWordEnd = lineStart;
			wordEnd = lineStart;
			pos = lineStart;
			++yOffset;
		}
	}

	// render any remaining text
	renderText(p, text.substr(lineStart), x, y + yOffset * charHeight, true);
}

void ui::text::setText(std::string_view value)
{
	text = value;
}

void ui::text::setSize(uint32_t size)
{
	this->size = size;
}

void ui::text::setWrapWidth(uint32_t wrapWidth)
{
	this->wrapWidth = wrapWidth;
}

void ui::text::setColor(const glm::vec4& color)
{
	this->color = color;
}

void ui::text::setShadow(bool shadow)
{
	this->shadow = shadow;
}

void ui::text::getBoundSize(int* w, int* h)
{
	int charWidth = tr.getCharWidth() * size;
	int charHeight = tr.getCharHeight() * size * 1.5;
	if (wrapWidth == 0)
	{
		// count the lines
		size_t maxLineLength = 0;
		size_t lineCount = 1;
		size_t lineStart = 0;
		size_t pos = 0;
		for (;;)
		{
			pos = glm::min(text.find('\n', pos), text.length());
			maxLineLength = glm::max(pos - lineStart, maxLineLength);

			if (pos == text.length())
			{
				break;
			}

			lineStart = pos;
			++pos;
			++lineCount;
		}

		*w = maxLineLength * charWidth;
		*h = lineCount * charHeight;

		return;
	}

	*w = wrapWidth;

	if (text.length() * charWidth <= wrapWidth)
	{
		*h = charHeight;
		return;
	}

	// calculate height
	size_t yOffset = 0;
	size_t lineStart = 0;
	size_t lastWordEnd = 0;
	size_t wordEnd = 0;
	size_t pos = 0;
	while (pos < text.length() - 1)
	{
		pos = glm::min(text.find(' ', pos), text.find('\n', pos));

		if (pos == std::string::npos)
		{
			pos = text.length() - 1;
		}

		lastWordEnd = wordEnd;
		wordEnd = pos;
		++pos;

		if (text[lastWordEnd] == '\n' || (wordEnd - lineStart) * charWidth > wrapWidth)
		{
			size_t lineEnd = (lastWordEnd == lineStart ? wordEnd : lastWordEnd);
			lineStart = lineEnd + 1;
			lastWordEnd = lineStart;
			wordEnd = lineStart;
			pos = lineStart;
			++yOffset;
		}
	}

	*h = (yOffset + 1) * charHeight;
}

void ui::text::renderText(page* p, std::string_view text, int x, int y, bool align)
{
	while (text[0] == '\n' || text[0] == '\r')
	{
		text = text.substr(1);
	}

	tr.setText(text);
	tr.align(false);
	tr.setFontSize(size);

	int adjustedX;
	int charWidth = tr.getCharWidth() * size;
	if (!align || alignmentX == ALIGN_LEFT)
	{
		adjustedX = x;
	}
	else if (alignmentX == ALIGN_CENTER_X)
	{
		adjustedX = x - (charWidth * text.length() / 2);
	}
	else
	{
		adjustedX = x - charWidth * text.length();
	}

	if (shadow)
	{
		tr.setColor(0, 0, 0);
		tr.setPos(adjustedX + size, y + size);
		tr.render();
	}

	tr.setColor(color);
	tr.setPos(adjustedX, y);
	tr.render();
}

void ui::image::getBounds(page* p, int* x, int* y, int* w, int* h)
{
	*w = width;
	*h = height;

	int wWidth, wHeight;
	glfwGetFramebufferSize(p->getWindow(), &wWidth, &wHeight);

	switch (alignmentX)
	{
	case ALIGN_LEFT:
	{
		*x = 0 + offsetX;
		break;
	}
	case ALIGN_RIGHT:
	{
		*x = wWidth - width + offsetX;
		break;
	}
	case ALIGN_CENTER_X:
		[[fallthrough]];
	default:
	{
		*x = ((wWidth - width) / 2) + offsetX;
		break;
	}
	}

	switch (alignmentY)
	{
	case ALIGN_TOP:
	{
		*y = 0 + offsetY;
		break;
	}
	case ALIGN_BOTTOM:
	{
		*y = wHeight - height + offsetY;
		break;
	}
	case ALIGN_CENTER_Y:
		[[fallthrough]];
	default:
	{
		*y = ((wHeight - height) / 2) + offsetY;
		break;
	}
	}
}

void ui::image::init(page* p)
{
	renderer.setShader(Shader::get("tex"));
	renderer.setScaleOrigin({ 0.0f, 0.0f });
}

void ui::image::render(page* p)
{
	renderer.render();
}

void ui::image::windowResize(page* p, int width, int height)
{
	int x, y, w, h;
	getBounds(p, &x, &y, &w, &h);
	renderer.setPos(x, y);
}

void ui::image::setScale(float x, float y)
{
	renderer.setScale(x, y);
	updateSize();
}

void ui::image::updateSize()
{
	const Texture* texture = renderer.getTexture();
	if (texture != nullptr)
	{
		auto& size = texture->getSize();
		auto scale = renderer.getScale();
		width = size.x * scale.x;
		height = size.y * scale.y;
	}
}

void ui::image::setTexture(const Texture* texture)
{
	renderer.setTexture(texture);
	updateSize();
}

void ui::button::getBounds(page* p, int* x, int* y, int* w, int* h)
{
	*w = width;
	*h = height;

	int wWidth, wHeight;
	glfwGetFramebufferSize(p->getWindow(), &wWidth, &wHeight);

	switch (alignmentX)
	{
	case ALIGN_LEFT:
	{
		*x = 0 + offsetX;
		break;
	}
	case ALIGN_RIGHT:
	{
		*x = wWidth - width + offsetX;
		break;
	}
	case ALIGN_CENTER_X:
		[[fallthrough]];
	default:
	{
		*x = ((wWidth - width) / 2) + offsetX;
		break;
	}
	}

	switch (alignmentY)
	{
	case ALIGN_TOP:
	{
		*y = 0 + offsetY;
		break;
	}
	case ALIGN_BOTTOM:
	{
		*y = wHeight - height + offsetY;
		break;
	}
	case ALIGN_CENTER_Y:
		[[fallthrough]];
	default:
	{
		*y = ((wHeight - height) / 2) + offsetY;
		break;
	}
	}
}

void ui::button::render(page* p)
{
	int x, y, w, h;
	getBounds(p, &x, &y, &w, &h);
	qr.setPos(x, y, w, h);

	constexpr glm::vec3 colorA{ 20 / 255.0f, 160 / 255.0f, 46 / 255.0f };
	constexpr glm::vec3 colorB{ 121 / 255.0f, 58 / 255.0f, 128 / 255.0f };

	glm::vec3 textColor{ 1.0f, 1.0f, 1.0f };

	if (mouseDown)
	{
		qr.setColor(textColor);
		textColor = colorA;
	}
	else
	{
		double mousex, mousey;
		glfwGetCursorPos(p->getWindow(), &mousex, &mousey);
		if (mousex >= x && mousex < x + w && mousey >= y && mousey < y + h)
		{
			qr.setColor(colorA);
		}
		else
		{
			qr.setColor(colorB);
		}
	}

	qr.render();

	tr.setText(text);
	int tw = tr.getCharWidth() * 2 * text.size();
	int th = tr.getCharHeight() * 2;

	int tx = x + ((w - tw) / 2);
	int ty = y + ((h - th) / 2);

	tr.setFontSize(2);

	// text shadow
	tr.setPos(tx + 2, ty + 2);
	tr.setColor({ 0.0f, 0.0f, 0.0f });
	tr.render();

	// text
	tr.setPos(tx, ty);
	tr.setColor(textColor);
	tr.render();
}

bool ui::button::mouseButtonInput(page* p, double xpos, double ypos, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		mouseDown = true;
		return true;
	}
	else if (action == GLFW_RELEASE)
	{
		if (mouseDown)
		{
			this->action();
			mouseDown = false;
			return true;
		}
	}
	return false;
}

void ui::button::mouseDownCancel()
{
	mouseDown = false;
}

void ui::button::defocus()
{
	mouseDown = false;
}

void ui::button::setAction(std::move_only_function<void()> action)
{
	this->action = std::move(action);
}

void ui::button::setText(std::string_view text)
{
	this->text = text;
}

void ui::button::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
}