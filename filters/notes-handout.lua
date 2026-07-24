-- Handout-PDF (LaTeX): je Folie (# / ##) als Kasten oben, Notizen darunter.
-- Foliennummer in der Titelzeile rechts (wie Reveal.js: n / N).

local function is_notes_div(el)
  return el.t == "Div" and el.classes:includes("notes")
end

function Pandoc(doc)
  -- Nur für LaTeX/PDF umstrukturieren
  if not FORMAT:match("latex") then
    return nil
  end

  local blocks = doc.blocks

  -- Wie Reveal.js: Level-1- und Level-2-Header zählen als Folien
  local total_slides = 0
  for _, b in ipairs(blocks) do
    if b.t == "Header" and b.level <= 2 then
      total_slides = total_slides + 1
    end
  end

  local new_blocks = pandoc.List()
  local i = 1
  local slide_num = 0

  while i <= #blocks do
    local b = blocks[i]

    if b.t == "Header" and b.level <= 2 then
      slide_num = slide_num + 1
      local slide_blocks = pandoc.List({ b })
      local note_blocks = pandoc.List()
      i = i + 1

      while i <= #blocks do
        local cur = blocks[i]
        -- Nächste Folie (Level 1 oder 2) beendet den aktuellen Block
        if cur.t == "Header" and cur.level <= 2 then
          break
        end
        if is_notes_div(cur) then
          for _, nb in ipairs(cur.content) do
            note_blocks:insert(nb)
          end
        else
          slide_blocks:insert(cur)
        end
        i = i + 1
      end

      -- Titelzeile: „Folie“ links, Nummer rechts (Reveal-Stil)
      local title = string.format(
        "Folie\\hfill\\textcolor{mutedgray}{\\mdseries\\normalsize %d\\,/\\,%d}",
        slide_num,
        total_slides
      )

      new_blocks:insert(pandoc.RawBlock("latex", "\\Needspace{12\\baselineskip}"))
      new_blocks:insert(pandoc.RawBlock("latex", "\\begin{folienblock}{" .. title .. "}"))
      for _, sb in ipairs(slide_blocks) do
        new_blocks:insert(sb)
      end
      new_blocks:insert(pandoc.RawBlock("latex", "\\end{folienblock}"))

      if #note_blocks > 0 then
        new_blocks:insert(pandoc.RawBlock("latex", "\\begin{notesblock}"))
        new_blocks:insert(pandoc.RawBlock(
          "latex",
          "{\\bfseries\\color{dhbwred}Notizen}\\par\\medskip"
        ))
        for _, nb in ipairs(note_blocks) do
          new_blocks:insert(nb)
        end
        new_blocks:insert(pandoc.RawBlock("latex", "\\end{notesblock}"))
      end
    else
      new_blocks:insert(b)
      i = i + 1
    end
  end

  doc.blocks = new_blocks
  return doc
end
