import java.awt.*;
import java.awt.event.*;

public class TestFrame extends Frame implements ActionListener {
	public TestFrame(){
		this("TestFrame", new Button("hello"));
	}
	public TestFrame(String title, Component c){
		super(title);
		MenuBar mb = new MenuBar();
		Menu file = new Menu("File");
		MenuItem quit = new MenuItem("Quit");
		file.add(quit);
		mb.add(file);
		setMenuBar(mb);
		
		quit.addActionListener(this);
		add(c,"Center");
		pack();
		show();
	}
	public void actionPerformed(ActionEvent e) {
		System.out.println(e);
		System.exit(0);
	}
	
	public static void main(String[] argv) {
		TestFrame t = new TestFrame();
	}
}
