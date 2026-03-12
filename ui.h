#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "QuadRenderer.h"
#include "TextRenderer.h"
#include "TexRenderer.h"

struct GLFWwindow;

namespace ui
{
	enum alignx
	{
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_CENTER_X,
	};

	enum aligny
	{
		ALIGN_TOP,
		ALIGN_BOTTOM,
		ALIGN_CENTER_Y,
	};
	
	class element;
	class page
	{
	public:

		using container_t = std::vector<element*>;

		void addElem(element* e);
		bool removeElem(element* e);
		void removeElem(container_t::iterator it);

		container_t::iterator begin();
		container_t::iterator end();

		container_t::const_iterator begin() const;
		container_t::const_iterator end() const;
		
		void init(GLFWwindow* w);
		void render();
		void mouseInput(double xpos, double ypos);
		void scrollInput(double xoff, double yoff);
		void mouseButtonInput(int button, int action, int mods);
		void keyInput(int key, int scancode, int action, int mods);
		void windowResize(int width, int height);
		void charInput(uint32_t codepoint);
		void fileDrop(int count, const char* paths[]);

		GLFWwindow* getWindow();
		// returns nullptr if there is no focused element
		element* getFocusedElem();

	private:

		container_t elems;
		GLFWwindow* window = nullptr;

		int focusIndex = -1;
	};
	
	class element
	{
	public:

		static void renderInit();

		// virtual methods
		// ---------------

		virtual bool clickable() { return false; }
		virtual void getBounds(page* p, int* x, int* y, int* w, int* h) = 0;

		virtual void init(page* p) {}
		virtual void render(page* p) {}
		virtual void mouseInput(page* p, double xpos, double ypos) {}
		virtual bool scrollInput(page* p, double xoff, double yoff) { return false; }
		virtual bool mouseButtonInput(page* p, double xpos, double ypos, int button, int action, int mods) { return false; }
		virtual void keyInput(page* p, int key, int scancode, int action, int mods) {}
		virtual void windowResize(page* p, int width, int height) {}
		virtual void charInput(page* p, uint32_t codepoint) {}
		virtual void fileDrop(page* p, int count, const char* paths[]) {}

		// non-virtual methods
		// -------------------

		alignx getAlignX() const;
		aligny getAlignY() const;

		void setAlignX(alignx alignment);
		void setAlignY(aligny alignment);

		int getOffsetX() const;
		int getOffsetY() const;

		void setOffsetX(int offset);
		void setOffsetY(int offset);

	protected:

		static QuadRenderer qr;
		static TextRenderer tr;

		alignx alignmentX;
		aligny alignmentY;

		int offsetX;
		int offsetY;
	};

	class text : public element
	{
	public:

		// element overrides
		// -----------------

		void getBounds(page* p, int* x, int* y, int* w, int* h) override;

		//void init(page* p) override;
		void render(page* p) override;

		// class-specific methods
		// ----------------------

		void setValue(std::string_view value);

		int getSize() const;
		void setSize(uint32_t size);

		// set to zero for no wrap width
		void setWrapWidth(uint32_t wrapWidth);

		void setColor(const glm::vec4& color);

		void setShadow(bool shadow);

	private:

		std::string text;
		uint32_t size = 2;
		uint32_t wrapWidth = 0;
		glm::vec4 color{ 1.0f };
		bool shadow = false;

		void getBoundSize(int* w, int* h);

		void renderText(page* p, std::string_view text, int x, int y, bool align);
	};

	class image : public element
	{
	public:

		// element overrides
		// -----------------

		void getBounds(page* p, int* x, int* y, int* w, int* h) override;

		void init(page* p) override;
		void render(page* p) override;

		// class-specific methods
		// ----------------------

		void setTexture(const Texture* texture, bool updateDimensions = true);
		void setDimensions(int w, int h);

	private:

		const Texture* texture = nullptr;
		TexRenderer renderer;

		int width;
		int height;
	};
}