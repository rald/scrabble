import java.awt.*;

/** DragTile **/
class DragTile extends Tile {
	static final Color XORMODE = Color.green;
	static final Color TEXTCOLOR = Color.black;
	
	Point pp;
	int	dx, dy;	// offset of drag start point from tile corner
	int	size;
	Graphics g;
	
	DragTile(Tile t, Point corner, Point clicked, int size, Graphics g) {
		super(t);
		this.g = g.create();
		this.g.setXORMode(XORMODE);
		pp = corner;
		dx = corner.x -clicked.x;
		dy = corner.y - clicked.y;
		this.size = size;
		paint(this.g, corner, size);
	}
	
	public void dragTo(Point p) {
		erase();
		pp.x = p.x + dx;
		pp.y = p.y + dy;
		draw();
	}

	public void paint() {
		paint(g, pp, size);
	}
	
	public void erase() {
		paint ();
	}
	
	public void draw() {
		paint();
	}
}
