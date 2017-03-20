#include "TextItem.h"

#include "RichEditor.h"
#include "TextEditor.h"
#include "ImageAttr.h"

TextItem::TextItem(ImageAttr* attr, Json* jnode):
	AttrItem{attr, Type::TEXT, tr("Text %1").arg(attr->applyId()), jnode}
{
	if (jnode) {
		jtov(html);
		jtov(letterSpace);
		jtov(lineSpace);
		jtov(paragraphSpace);
	}
}

void TextItem::exportProjJson(Json& jnode)
{
	AttrItem::exportProjJson(jnode);
	vtoj(html);
	vtoj(letterSpace);
	vtoj(lineSpace);
	vtoj(paragraphSpace);
}

bool TextItem::editImpl()
{
	TextEditor editor{listWidget(), attr->contentRect->size, this};
	return QDialog::Accepted == editor.exec();
}

void TextItem::printImpl()
{
	QSize size = attr->contentRect->size;
	pages.clear();
	editingPage = 0;

	QTextDocument doc;
	doc.setHtml(html);
	doc.setTextWidth(size.width());
	doc.setDocumentMargin(0);
	RichEditor::setDocLetterSpace(&doc, letterSpace);
	RichEditor::setDocLineSpace(&doc, lineSpace);
	RichEditor::setDocParagraphSpace(&doc, paragraphSpace);
	QPixmap snapshot = RichEditor::snapshot(&doc);

	for (int y = 0; y < snapshot.height(); y+=size.height()) {
		QPixmap page{size};
		page.fill(Qt::transparent);
		QPainter painter{&page};
		painter.drawPixmap(QPoint{0, 0}, snapshot, {{0, y}, size});
		pages.push_back(page);
	}
}
