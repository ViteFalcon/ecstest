/*
------------------------------------------------------------------------------------------------------------------------
Copyright 2019 Vite Falcon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------------------------------
*/

#ifndef NINPOTEST_GAMEUI_H
#define NINPOTEST_GAMEUI_H

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Container/HashSet.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>

class GameUI : public Urho3D::Object {
URHO3D_OBJECT(GameUI, Urho3D::Object)
public:
  GameUI(Urho3D::Context* context);
  virtual ~GameUI();

protected:
  void addElement(Urho3D::UIElement* element);

  void setNamedElement(Urho3D::String name, Urho3D::UIElement* element, bool addAsRootElement=false);

  template <typename T>
  T* getNamedElement(Urho3D::String name) {
    auto element = mNamedElement[name];
    return static_cast<T*>(element);
  }

private:
  Urho3D::UI& getUI();

  Urho3D::HashMap<Urho3D::String, Urho3D::UIElement*> mNamedElement;
  Urho3D::HashSet<Urho3D::UIElement*> mElements;
};

#define GAME_UI(ClassName) URHO3D_OBJECT(ClassName, GameUI)

#endif //NINPOTEST_GAMEUI_H
