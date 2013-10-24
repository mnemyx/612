import maya.cmds as mc
import maya.mel as mm
import math

data = []
last_eyer_data = []
last_eyel_data = []

rightblink = 0
last_frame = -1
frame_count = 0

#filelocation = 'C:/Users/mnemy_000/Documents/___DPA_FALL2013/CPSC612/JohnsEyeData/JH_T3_TENTHSPEED'
#filelocation = 'C:/Users/mnemy_000/Documents/___DPA_FALL2013/CPSC612/GinasEyeData/GG_TENTHSPEED2'
filelocation = 'C:/Users/mnemy_000/Documents/___DPA_FALL2013/CPSC612/AD_T3_HALFSPEED'

def converteyedata(x, y, z):
    #xangle = math.degrees(math.atan(math.radians(float(x))/math.radians(float(z))))
    #yangle = math.degrees(math.atan(math.radians(float(y))/math.radians(float(z))))
    #yangle = math.degrees(math.atan(float(x)/float(z)))
    #xangle = math.degrees(math.atan(float(y)/float(z)))
    yangle = -math.degrees(math.atan2(float(x),float(z)))
    xangle = math.degrees(math.atan2(float(y),float(z)))
    return (xangle, yangle)

def parseline(line):
    stringsplit = line.split(',')
    return stringsplit[:-1]
    
def doheadrotate(head_data, frame_number):
    if head_data[7]=='False':
        mc.setKeyframe('head', attribute='rotateX', t=int(frame_number), v=float(head_data[4]))
        mc.setKeyframe('head', attribute='rotateY', t=int(frame_number), v=float(head_data[5]))
        mc.setKeyframe('head', attribute='rotateZ', t=int(frame_number), v=float(head_data[6]))

def doeyerotate(obj, eye_data, frame_number):
    (xang,yang)=converteyedata(eye_data[4], eye_data[6], eye_data[8])
    #print obj, ' xang: ', xang
    #print obj, ' yang: ', yang
    #mc.getRotatePivot(space=obj)
    #mc.autoKeyframe(state=true)
    #mc.currentTime(int(frame_number), update=false)
    #mc.rotate(obj, objectCenterPivot=true, rotatex=xang, rotatey=yang)
    #mc.autoKeyframe()
    mc.setKeyframe(obj, attribute='rotateX', t=int(frame_number), v=xang)
    mc.setKeyframe(obj, attribute='rotateY', t=int(frame_number), v=yang)
    #mc.setKeyframe(obj, attribute='rotateX', t=int(frame_number), v=float(eye_data[4]))
    #mc.setKeyframe(obj, attribute='rotateY', t=int(frame_number), v=float(eye_data[6]))

def doblink(obj, frame_number):
    mc.setKeyframe(obj, attribute='rotateX', t=int(frame_number), v=75)
    
def dounblink(obj, frame_number):
    mc.setKeyframe(obj, attribute='rotateX', t=int(frame_number), v=0)

print 'starting!'
with open(filelocation) as myfile:
    for line in myfile:
        parsed_line = parseline(line)
        
        frame_number = parsed_line[0]
        
        if frame_number=='335544320':
            continue
            
        doheadrotate(parsed_line[3:11], frame_number)
        
        try:
            i = parsed_line.index('Eye-R')
            if parsed_line[i+1]!= 'Eye-L':
                #print 'i have right eye data'
                dounblink('EyelidR', frame_number)
                righblink = 0
                doeyerotate('EyeR', parsed_line[i+1:i+11], frame_number)
            else:
                #print frame_number + ': right eye blink'
                rightblink = 1
        except:
            continue
            
        try:
            i = parsed_line.index('Eye-L')
            if parsed_line[i+1]!= 'EyePG':
                #print 'i have left eye data'
                dounblink('EyelidL', frame_number)
                doeyerotate('EyeL', parsed_line[i+1:i+11], frame_number)
            else:
                #print frame_numer + ': left eye blink'
                if rightblink == 1:
                    doblink('EyelidL', frame_number)
                    doblink('EyelidR', frame_number)
        except:
            continue
        
        frame_count = frame_count + 1
        
        if last_frame < frame_number:
            last_frame = frame_number
            
        #data.append(parsed_line)
        
#for d in data:
#    print d
        

print 'done! frame count: ', frame_count


