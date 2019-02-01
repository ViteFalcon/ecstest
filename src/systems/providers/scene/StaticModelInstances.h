/*
------------------------------------------------------------------------------------------------------------------------
Copyright 2019 Vite Falcon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------------------------------
*/

#ifndef NINPOTEST_STATICMESHINSTANCES_H
#define NINPOTEST_STATICMESHINSTANCES_H

#include "NodeComponentInstances.h"
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "../../../components/StaticModel.h"

class StaticModelInstances
    : public NodeComponentInstances<StaticModelInstances, StaticModel,
                                    Urho3D::StaticModel> {
public:
  StaticModelInstances(Urho3D::Scene &scene, NodeInstances nodes,
                       Urho3D::ResourceCache &resources);

private:
  virtual Urho3D::SharedPtr<Urho3D::StaticModel>
  CreateNodeComponent(entityx::Entity entity, Urho3D::Node &node,
                      const StaticModel &component,
                      entityx::EntityManager &entities) override;

  virtual void SyncFromData(entityx::Entity entity,
                            Urho3D::StaticModel &instance,
                            const StaticModel &data) override;

  Urho3D::ResourceCache &mResources;
};

#endif // NINPOTEST_STATICMESHINSTANCES_H
