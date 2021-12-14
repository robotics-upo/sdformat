/*
 * Copyright 2021 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "sdf/Conversions.hh"

#include <ignition/common/Pbr.hh>
#include <sdf/Pbr.hh>

namespace sdf
{
  inline namespace SDF_VERSION_NAMESPACE {

  sdf::Material convert(const ignition::common::Material &_in)
  {
    sdf::Material out;
    out.SetEmissive(_in.Emissive());
    out.SetDiffuse(_in.Diffuse());
    out.SetSpecular(_in.Specular());
    out.SetAmbient(_in.Ambient());
    out.SetRenderOrder(_in.RenderOrder());
    out.SetLighting(_in.Lighting());
    out.SetDoubleSided(_in.TwoSidedEnabled());
    ignition::common::Pbr * pbr = _in.PbrMaterial();
    if (pbr != nullptr)
    {
      out.SetNormalMap(pbr->NormalMap());
      sdf::Pbr pbrOut;
      sdf::PbrWorkflow pbrWorkflow;
      pbrWorkflow.SetAlbedoMap(pbr->AlbedoMap());
      pbrWorkflow.SetMetalnessMap(pbr->MetalnessMap());
      pbrWorkflow.SetEmissiveMap(pbr->EmissiveMap());
      pbrWorkflow.SetRoughnessMap(pbr->RoughnessMap());

      pbrWorkflow.SetEnvironmentMap(pbr->EnvironmentMap());
      pbrWorkflow.SetAmbientOcclusionMap(pbr->AmbientOcclusionMap());
      pbrWorkflow.SetLightMap(pbr->LightMap());

      if (pbr->NormalMapType() == ignition::common::NormalMapSpace::TANGENT)
      {
        pbrWorkflow.SetNormalMap(
          pbr->NormalMap(), sdf::NormalMapSpace::TANGENT);
      }
      else
      {
        pbrWorkflow.SetNormalMap(
          pbr->NormalMap(), sdf::NormalMapSpace::OBJECT);
      }

      pbrWorkflow.SetRoughness(pbr->Roughness());
      pbrWorkflow.SetGlossiness(pbr->Glossiness());
      pbrWorkflow.SetMetalness(pbr->Metalness());

      if (pbr->Type() == ignition::common::PbrType::METAL)
      {
        pbrOut.SetWorkflow(sdf::PbrWorkflowType::METAL, pbrWorkflow);
      }
      else if (pbr->Type() == ignition::common::PbrType::SPECULAR)
      {
        pbrOut.SetWorkflow(sdf::PbrWorkflowType::SPECULAR, pbrWorkflow);
      }
      out.SetPbrMaterial(pbrOut);
    }

    return out;
  }

  std::shared_ptr<ignition::common::Material> convertMaterial(sdf::Material &_in)
  {
    std::shared_ptr<ignition::common::Material> out =
      std::make_shared<ignition::common::Material>();
    out->SetEmissive(_in.Emissive());
    out->SetEmissive(_in.Emissive());
    out->SetDiffuse(_in.Diffuse());
    out->SetSpecular(_in.Specular());
    out->SetAmbient(_in.Ambient());
    out->SetRenderOrder(_in.RenderOrder());
    out->SetLighting(_in.Lighting());
    // TODO(ahcorde): Review this
    // out->SetTwoSidedEnabled(_in.DoubleSided());

    const sdf::Pbr * pbr = _in.PbrMaterial();
    if (pbr != nullptr)
    {
      ignition::common::Pbr pbrOut;

      const sdf::PbrWorkflow * pbrWorkflow =
        pbr->Workflow(sdf::PbrWorkflowType::METAL);
      if (pbrWorkflow)
      {
        pbrOut.SetType(ignition::common::PbrType::METAL);
      }
      else
      {
        pbrWorkflow = pbr->Workflow(sdf::PbrWorkflowType::SPECULAR);
        if (pbrWorkflow)
        {
          pbrOut.SetType(ignition::common::PbrType::SPECULAR);
        }
      }
      if (pbrWorkflow != nullptr)
      {
        pbrOut.SetAlbedoMap(pbrWorkflow->AlbedoMap());
        pbrOut.SetMetalnessMap(pbrWorkflow->MetalnessMap());
        pbrOut.SetEmissiveMap(pbrWorkflow->EmissiveMap());
        pbrOut.SetRoughnessMap(pbrWorkflow->RoughnessMap());
        pbrOut.SetEnvironmentMap(pbrWorkflow->EnvironmentMap());
        pbrOut.SetAmbientOcclusionMap(pbrWorkflow->AmbientOcclusionMap());
        pbrOut.SetLightMap(pbrWorkflow->LightMap());
        pbrOut.SetRoughness(pbrWorkflow->Roughness());
        pbrOut.SetGlossiness(pbrWorkflow->Glossiness());
        pbrOut.SetMetalness(pbrWorkflow->Metalness());

        if (pbrWorkflow->NormalMapType() == sdf::NormalMapSpace::TANGENT)
        {
          pbrOut.SetNormalMap(
            pbrWorkflow->NormalMap(),
            ignition::common::NormalMapSpace::TANGENT);
        }
        else if(pbrWorkflow->NormalMapType() == sdf::NormalMapSpace::OBJECT)
        {
          pbrOut.SetNormalMap(
            pbrWorkflow->NormalMap(),
            ignition::common::NormalMapSpace::OBJECT);
        }
      }
      out->SetPbrMaterial(pbrOut);
    }
    return out;
  }
}
}