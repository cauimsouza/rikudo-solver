import java.awt.*;
import java.util.Collections;
import java.util.LinkedList;

import javax.swing.JComponent;
import javax.swing.JFrame;

// Manipulation for images
public class Image2d {
	private int width; // width of the image
	private int height; // height of the image
	private java.util.List<ColoredPolygon> coloredPolygons; // colored polygons in the image
	private java.util.List<ColoredSegment> coloredSegments; // colored segments in the image
	private java.util.List<Text> texts; // vertices' labels to write

	// Constructor that instantiates an image of a specified width and height
	public Image2d(int width, int height) {
		this.width = width;
		this.height = height;
		coloredPolygons = Collections.synchronizedList(new LinkedList<ColoredPolygon>());
		coloredSegments = Collections.synchronizedList(new LinkedList<ColoredSegment>());
		texts = Collections.synchronizedList(new LinkedList<Text>());
	}

	// Constructor that instantiates an image of a specified size
	public Image2d(int size) {
		this.width = size;
		this.height = size;
		coloredPolygons = Collections.synchronizedList(new LinkedList<ColoredPolygon>());
		coloredSegments = Collections.synchronizedList(new LinkedList<ColoredSegment>());
		texts = Collections.synchronizedList(new LinkedList<Text>());
	}

	// Return the width of the image
	public int getWidth() {
		return width;
	}

	// Return the height of the image
	public int getHeight() {
		return height;
	}

	// Return the colored polygons of the image
	public java.util.List<ColoredPolygon> getColoredPolygons() {
		return coloredPolygons;
	}

	// Return the colored segments of the image
	public java.util.List<ColoredSegment> getColoredSegments() {
		return coloredSegments;
	}
	
	// Return the text labels of the image
	public java.util.List<Text> getTexts() {
		return texts;
	}

	// Create polygon with xcoords, ycoords and colors insideColor, boundaryColor
	public void addPolygon(int[] xcoords, int[] ycoords, Color insideColor, Color boundaryColor) {
		coloredPolygons.add(new ColoredPolygon(xcoords, ycoords, insideColor, boundaryColor));
	}
	
	// Create hexagon with offset coordinates given by Offset and colors insideColor, boundaryColor
	public void addHexagon(Offset center, Color insideColor, Color boundaryColor) {
		coloredPolygons.add(new ColoredPolygon(new Hexagon(center), insideColor, boundaryColor));
	}
	
	// Create hexagon with axial coordinates given by cube and colors insideColor, boundaryColor
	public void addHexagon(Axial center, Color insideColor, Color boundaryColor) {
		addHexagon(center.toOffset(100), insideColor, boundaryColor);
	}
	
	// Create segment from (x1,y1) to (x2,y2) with some given width and color 
	public void addSegment(int x1, int y1, int x2, int y2, int width, Color color) {
		coloredSegments.add(new ColoredSegment(x1, y1, x2, y2, width, color));
	}
	
	public void addText(Text text) {
		texts.add(text);
	}
	
	// Clear the picture
	public void clear() {
		coloredPolygons = Collections.synchronizedList(new LinkedList<ColoredPolygon>());
		coloredSegments = Collections.synchronizedList(new LinkedList<ColoredSegment>());
	}
}

//Frame for the vizualization
class Image2dViewer extends JFrame {

	private static final long serialVersionUID = -7498525833438154949L;
	static int xLocation = 0;
	public Image2d img;

	public Image2dViewer(Image2d img) {
		this.img = img;
		this.setLocation(xLocation, 0);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		add(new Image2dComponent(img));
		pack();
		setVisible(true);
		xLocation += img.getWidth();
	}
}

//Image2d component
class Image2dComponent extends JComponent {

	private static final long serialVersionUID = -7710437354239150390L;
	private Image2d img;

	public Image2dComponent(Image2d img) {
		this.img = img;
		setPreferredSize(new Dimension(img.getWidth(), img.getHeight()));
	}

	public void paint(Graphics g) {
		Graphics2D g2 = (Graphics2D) g;

		// set the background color
		Dimension d = getSize();
		g2.setBackground(Color.white);
		g2.clearRect(0, 0, d.width, d.height);
		
		// set the font
		g2.setFont(new Font("Courier", Font.PLAIN, 40));

		// draw the polygons
		synchronized (img.getColoredPolygons()) {
			for (ColoredPolygon coloredPolygon : img.getColoredPolygons()) {
				g2.setColor(coloredPolygon.insideColor);
				g2.fillPolygon(coloredPolygon.polygon);
				g2.setColor(coloredPolygon.boundaryColor);
				g2.drawPolygon(coloredPolygon.polygon);
			}
		}
		
		// draw the edges
		synchronized (img.getColoredSegments()) {
			for (ColoredSegment coloredSegment : img.getColoredSegments()) {
				g2.setColor(coloredSegment.color);
				g2.setStroke(new BasicStroke(coloredSegment.width));
				g2.drawLine(coloredSegment.x1, coloredSegment.y1, coloredSegment.x2, coloredSegment.y2);
			}
		}
		
		// draw the texts
		synchronized (img.getTexts()) {
			for (Text text : img.getTexts()) {
				g2.drawString(text.label, text.x - 15, text.y+10);
			}
		}
	}
}