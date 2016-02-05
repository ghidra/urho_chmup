Viewport@ viewport = renderer.viewports[0];
RenderPath@ effectRenderPath = viewport.renderPath.Clone();
effectRenderPath.Append(cache.GetResource("XMLFile", "RenderPaths/Glow.xml"));
effectRenderPath.SetEnabled("Glow", true);
viewport.renderPath = effectRenderPath;
renderer.viewports[0] = viewport;