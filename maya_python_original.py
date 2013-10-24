import maya.cmds as mc
import maya.mel as mm

melproc = """
global proc portData(string $arg){
    python(("portData(\\"" + $arg + "\\")"));
}
"""
mm.eval(melproc)

def portData(arg):
    """
    Read the 'serial' data passed in from the commandPort
    """
    arg = arg.split(",")
    print "From Vicon: ", arg
    
    mc.setKeyframe('head', attribute='rotateX', t=float(arg[1]), v=float(arg[4]))
    mc.setKeyframe('head', attribute='rotateY', t=float(arg[1]), v=float(arg[5]))
    mc.setKeyframe('head', attribute='rotateZ', t=float(arg[1]), v=float(arg[6]))
    mc.setKeyframe('EyeL', attribute='rotateX', t=float(arg[1]), v=float(arg[11]))
    mc.setKeyframe('EyeL', attribute='rotateY', t=float(arg[1]), v=float(arg[12]))
    mc.setKeyframe('EyeR', attribute='rotateX', t=float(arg[1]), v=float(arg[15]))
    mc.setKeyframe('EyeR', attribute='rotateY', t=float(arg[1]), v=float(arg[16]))


mc.commandPort(name="198.21.234.139:7777", echoOutput=False, noreturn=False,
               prefix="portData", returnNumCommands=True)
mc.commandPort(name=":7777", echoOutput=False, noreturn=False,
               prefix="portData", returnNumCommands=True)
