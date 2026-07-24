-- Handout-PDF: Bilder nicht auf volle Textbreite aufblasen.
-- Mermaid-PNGs und SVG-Schaltungen behalten ihre natuerliche Groesse;
-- LaTeX-adjustbox in _quarto-handout.yml begrenzt nur das Maximum.

function Image(el)
  if not FORMAT:match("latex") then
    return nil
  end
  -- Explizite width=100% aus Quarto/Mermaid entfernen (sonst Stretch auf \linewidth)
  el.attributes["width"] = nil
  el.attributes["height"] = nil
  return el
end
