from tkinter import *
import sys

class BaseDialog( Toplevel ):
    """
    Helper base class for dialogs used in the UI.
    """

    def __init__(self, parent, title = None, buttons=""):
        """
        Constructor
        """
        Toplevel.__init__( self, parent )
        self.transient(parent)

        if title:
            self.title(title)

        self.parent = parent
        self.result = None
        body = Frame(self)
        self.initial_focus = self.body(body)
        body.pack(padx=5, pady=5)
        self.buttonbox(buttons)
        self.grab_set()
        if not self.initial_focus:
            self.initial_focus = self
        self.protocol("WM_DELETE_WINDOW", self.cancel)
        self.geometry("+%d+%d" % (parent.winfo_rootx() + 50,
                                  parent.winfo_rooty() + 50))
        self.initial_focus.focus_set()
        self.wait_window(self)

    def body(self, master):
        # create dialog body.  return widget that should have
        # initial focus.  this method should be overridden
        pass

    def buttonbox(self, buttons):
        # add standard button box. override if you don't want the
        # standard buttons
        box = Frame(self)
        w = Button(box, text="OK", width=40, command=self.ok, default=ACTIVE)
        w.pack(side=LEFT, padx=5, pady=5)
        self.bind("<Return>", self.ok)
        box.pack()

    def ok(self, event=None):
        if not self.validate():
            self.initial_focus.focus_set()  # put focus back
            return

        self.withdraw()
        self.update_idletasks()
        self.apply()
        self.cancel()

    def cancel(self, event=None):
        # put focus back to the parent window
        self.parent.focus_set()
        self.destroy()

    def validate(self):
        return 1  # override

    def apply(self):
        pass  # override

class SetupDialog( BaseDialog ):
    """
    This class is used to create the setup dialog.
    """
    def body(self, master):
        Label(master, justify=LEFT, text="Project name: " ).grid(row=0, column=0)
        Label(master, justify=LEFT, text="Name").grid(row=0, column=1)
        self.e1 = Entry(master)
        self.e1.grid(row=1, column=1)
        return self.e1  # initial focus

    def apply(self):
        return 0


class ProjGenDialog( object ):
    """
    This class is used to create a simplified user interface for running the regression test suite.
    """
    
    def __init__(self, bin_path ):
        """
        Constructs the dialog, you can define which executable shal be used.
        @param  bin_path    [in] Path to assimp binary.
        """
        self.width=60


    def setup(self):
        d = SetupDialog( self.root )


    def generate(self):
        pass
    
    def quit(self):
        sys.exit(0)

    def initUi(self):
        # create the frame with buttons
        self.root = Tk()
        self.root.title( "OSRE project generator V0.1")
        self.setup = Button( self.root, text="Setup project", command=self.setup, width = self.width )
        self.generate = Button(self.root, text="Generate project", command=self.generate, width=self.width)
        self.quit = Button( self.root, text="Quit", command=self.quit, width = self.width )
        self.setup.grid(row=0, column=0, sticky=W+E )
        self.generate.grid(row=1, column=0, sticky=W+E)
        self.quit.grid(row=1, column=0, sticky=W + E)
        self.root.mainloop()

def main():
    project_name = ""
    project_dir  = "./"
    if len(sys.argv) > 1:
        project_name = sys.argv[1]

    dlg = ProjGenDialog( project_name )
    dlg.initUi()

if __name__ == "__main__":
    main()
