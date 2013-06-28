CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     AUXILIARY ROUTINES FOR Q-PYTHIA version 1.0.3.
C     Change with respect to v1.0: no link with CERNLIB required.
C     Change with respect to v1.0.1: problem of multiple calls to
C        qpygin solved. Now the user must call routine qpygin0 once
C        per nucleon-nucleon collision and define there the initial
C        position and time in the cms of the collision.
C        Do not touch qpygin which automatically makes the boost and
C        rotation to the cms of the hard scattering.
C     Change with respect to v1.0.2: geometries by Konrad Tywoniuk added.
C
C     DATE: 10.03.2011.
C
C     AUTHORS: N. Armesto, L. Cunqueiro and C. A. Salgado
C              Departamento de Fisica de Particulas and IGFAE
C              Universidade de Santiago de Compostela
C              15706 Santiago de Compostela, Spain
C     
C     EMAILS: nestor@fpaxp1.usc.es, leticia@fpaxp1.usc.es, 
C             Carlos.Salgado@cern.ch
C
C     CONTENT: auxiliary files for modified QPYSHOW, fixed to PYTHIA-6.4.18.
C              NOT to be modified by user.
C
C     WHEN USING Q-PYTHIA, PLEASE QUOTE:
C
C     1) N. Armesto, L. Cunqueiro and C. A. Salgado,
C        Eur. Phys. J. C63 (2009) 679 [arXiv:0907.1014 [hep-ph]].
C     2) T. Sjostrand, S. Mrenna and P. Skands,
C        ``PYTHIA 6.4 physics and manual,''
C        JHEP 0605 (2006) 026 [arXiv:hep-ph/0603175].
C
C     DISCLAIMER: this program comes without any guarantees. Beware of
C                 errors and use common sense when interpreting results.
C                 Any modifications are done under exclusive
C                 maker's resposibility.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
c     VERSION WITH THE LARGE X BEHAVIOR OF THE MEDIUM PART INTRODUCED
C     BY MULTIPLYING BY THE NUMERATOR OF THE COLLINEAR PART OF THE
C     VACUUM SPLITTING FUNCTION
c
      function splitq1(w)
c     to integrate, adding vaccum plus medium q -> qg
      implicit double precision (a-h,o-z)
      z=w
      auxz=z*(1.d0-z)
      auxq=splitgq(z)+splitmedq1(z)
      if (auxq .gt. 0.d0) then
         splitq1=auxq
      else
         splitq1=0.d0
      endif
      return
      end
c
      function splitg1(w)
c     to integrate, adding vaccum plus medium g -> gg, and g -> qqbar
      implicit double precision (a-h,o-z)
      z=w
      auxz=z*(1.d0-z)
c     argument of running coupling is taken as kt of emission
      auxg=splitgg(z)+splitmedg1(z)
      if (auxg .gt. 0.d0) then
         splitg1=(auxg+splitqqbar(z))
      else
         splitg1=splitqqbar(z)
      endif
      return
      end
c
      function splitq2(w)
c     to integrate, adding vaccum plus medium q -> qg
      implicit double precision (a-h,o-z)
      z=w
      auxq=splitgq(z)+splitmedq2(z)
      if (auxq .gt. 0.d0) then
         splitq2=auxq
      else
         splitq2=0.d0
      endif
      return
      end
c
      function splitg2(z)
c     to integrate, adding vaccum plus medium g -> gg, and g -> qqbar
      implicit double precision (a-h,o-z)
      auxg=splitgg(z)+splitmedg2(z)
      if(auxg.gt.0.d0) then
      splitg2=auxg
      else
      splitg2=0.d0
      endif 
      return
      end
c
      function splitgq(z)
c     q -> qg splitting kernel at 1 loop for the vacuum
      implicit double precision (a-h,o-z)
      xnc=3.d0  
      splitgq=(0.5d0*(xnc-1.d0/xnc))*(1.d0+z*z)/(1.d0-z)
      return
      end
c
      function splitgg(z)
c     g -> gg splitting kernel at 1 loop for the vacuum
      implicit double precision (a-h,o-z)
      xnc=3.d0
      auxz=z*(1.d0-z)
      auxz2=1.d0-auxz
      splitgg=xnc*auxz2*auxz2/auxz
      return
      end
c
      function splitqqbar(z)
c     g -> qqbar splitting kernel at 1 loop
      implicit double precision (a-h,o-z)
      xnf=5.d0
      auxz=1.d0-z
      splitqqbar=0.5d0*xnf*(z*z+auxz*auxz)
      return
      end
c
      function splitmedg1(z)
c     g -> gg splitting kernel at 1 loop for the medium
      implicit double precision (a-h,o-z)
      common/qpc1/eee,qhatl,omegac
      common/qpvir1/pmed
      xnc=3.d0
      pi=dacos(-1.d0)
      if (qhatl .le. 0.d0 .or. omegac .le. 0.d0) then
         splitmedg1=0.d0
      else
c     symmetrized by hand with respect to 1/2
         if (z .ge. 0.5d0) then
            zz=z
         else
            zz=1.d0-z
         endif
         t=pmed*pmed
         auxz=1.d0-zz
         auxz2=zz*auxz
         ome=eee*auxz/omegac
         xkappa2=auxz2*t/qhatl
         fff=genspec(ome,xkappa2)
cc     1/2 to avoid double counting
c         splitmedg=0.5d0*xnc*2.d0*pi*zz*t*fff/qhatl
c     we multiply by max(z,1-z) to introduce the large z behavior from the
c     numerator in the vacuum
         flx=max(zz,auxz)
c     1/2 to avoid double counting
         splitmedg1=0.5*flx*xnc*2.d0*pi*zz*t*fff/qhatl
      endif
      return
      end
c
      function splitmedq1(z)
c     q -> qg splitting kernel at 1 loop for the medium
      implicit double precision (a-h,o-z)
      common/qpc1/eee,qhatl,omegac
      common/qpvir1/pmed 
      xnc=3.d0
      pi=dacos(-1.d0)
      if (qhatl .le. 0.d0 .or. omegac .le. 0.d0) then
         splitmedq1=0.d0
      else
         t=pmed*pmed
         auxz=1.d0-z
         auxz2=z*auxz
         ome=eee*auxz/omegac
         xkappa2=auxz2*t/qhatl
         fff=genspec(ome,xkappa2)
c         splitmedq=(0.5d0*(xnc-1.d0/xnc))*2.d0*pi*z*t*fff/qhatl
c     we multiply by 1+z**2 to introduce the large z behavior from the
c     numerator in the vacuum
         flx=0.5d0*(1.d0+z*z)
         splitmedq1=flx*(0.5d0*(xnc-1.d0/xnc))*2.d0*pi*z*t*fff/qhatl
      endif
      return
      end
c
      function splitmedg2(z)
c     g -> gg splitting kernel at 1 loop for the medium
      implicit double precision (a-h,o-z)
      common/qpc1/eee,qhatl,omegac
      common/qpvir2/virt
      xnc=3.d0
      pi=dacos(-1.d0)
      if (qhatl .le. 0.d0 .or. omegac .le. 0.d0) then
         splitmedg2=0.d0
      else
c     symmetrized by hand with respect to 1/2
         if (z .ge. 0.5d0) then
            zz=z
         else
            zz=1.d0-z
         endif
         t=virt
         auxz=1.d0-zz
         auxz2=zz*auxz
         ome=eee*auxz/omegac
         xkappa2=auxz2*t/qhatl
         fff=genspec(ome,xkappa2)
cc     1/2 to avoid double counting
c         splitmedg=0.5d0*xnc*2.d0*pi*zz*t*fff/qhatl
c     we multiply by max(z,1-z) to introduce the large z behavior from the
c     numerator in the vacuum
         flx=max(zz,auxz)
c     1/2 to avoid double counting
         splitmedg2=0.5*flx*xnc*2.d0*pi*zz*t*fff/qhatl
      endif
      return
      end
c
      function splitmedq2(z)
c     q -> qg splitting kernel at 1 loop for the medium
      implicit double precision (a-h,o-z)
      common/qpc1/eee,qhatl,omegac
      common/qpvir2/virt 
      xnc=3.d0
      pi=dacos(-1.d0)
      if (qhatl .le. 0.d0 .or. omegac .le. 0.d0) then
         splitmedq2=0.d0
      else
         t=virt
         auxz=1.d0-z
         auxz2=z*auxz
         ome=eee*auxz/omegac
         xkappa2=auxz2*t/qhatl
         fff=genspec(ome,xkappa2)
c         splitmedq=(0.5d0*(xnc-1.d0/xnc))*2.d0*pi*z*t*fff/qhatl
c     we multiply by 1+z**2 to introduce the large z behavior from the
c     numerator in the vacuum
         flx=0.5d0*(1.d0+z*z)
         splitmedq2=flx*(0.5d0*(xnc-1.d0/xnc))*2.d0*pi*z*t*fff/qhatl
      endif
      return
      end
c
      function genspec(ome,xk2)
C     THIS FUNCTION GENERATES (omega/omegac) dI/d(omega/omegac) dkappa2,
C     omegac=qhat L**2/2, kappa2=kt2/(qhat L), in the mss approximation for m=0,
c     using interpolation and extrapolation. It reads file grid-qp.dat.
c     ome=omega/omegac, xk2=kappa2.
C     MAXIMUM GRID 101 TIMES 101, MODIFY ARRAY DIMENSIONS IF EXCEEDED.
c     alphas=1, cr=1.
      implicit double precision (a-h,o-z)
      character *4096 FILEINPUT 
      dimension xkap2(101), xlkap2(101), xome(101), xlome(101)
      dimension xspec(101,101)
      dimension aux1(101), aux2(101)
      COMMON/QOPTIONS/IGEO,XLFIX,QHATFIX,FILEINPUT
      save xkap2, xlkap2, xome, xlome, xspec, npkap, npome
      DATA IFLAG/0/
c     WE READ THE GRID ONLY THE FIRST TIME.
      IF (IFLAG .EQ. 0) THEN
c         print*, 'reading grid-qp.dat'
c         open(11,file='grid-qp.dat',status='old')
C     CMSSW BEGIN
         print *, FILEINPUT
         open(11,file=FILEINPUT,status='old')
C     CMSSW END
         read(11,*) npkap
         read(11,*) npome
         npkap=npkap+1
         npome=npome+1
         do 10 i=1, npkap, 1
            read(11,*) xkap2(i), xlkap2(i)
10       continue
         do 20 i=1, npome, 1
            read(11,*) xome(i), xlome(i)
20       continue
         do 30 j=1, npome, 1
            do 40 i=1, npkap, 1
               read(11,*) xspec(i,j)
40          continue
30       continue
         close(11)
         iflag=1
      ENDIF
c     cases
c     for ome>largest value set to 0,
c     for xk2< smallest value frozen,
c     for xk2> largest value 1/kappa4 extrapolation.
      if (ome .gt. xome(npome)) then
         genspec=0.d0
      elseif (ome .lt. xome(1)) then
         scal=.05648d0*dexp(1.674d0*ome)*dlog(.136d0/ome)/(ome**.5397d0)
         scal=0.25d0*9.d0*scal/xspec(1,1)
         if (xk2 .le. xkap2(1)) then
            genspec=scal*xspec(1,1)
         elseif (xk2 .eq. xkap2(npkap)) then
            genspec=scal*xspec(npkap,1)
         elseif (xk2 .gt. xkap2(npkap)) then
            genspec=scal*xspec(npkap,1)*
     >              xkap2(npkap)*xkap2(npkap)/(xk2*xk2)
         else
            do 50 i=1, npkap, 1
               aux1(i)=xspec(i,1)
50          continue
            genspec=scal*ddivdif(aux1,xlkap2,npkap,dlog(xk2),4)
         endif 
      else
         iexact=-1
         if (ome .eq. xome(1)) then
            iexact=1
            goto 70
         else
            do 60 i=1, npome-1, 1
               if (ome .eq. xome(i+1)) then
                  iexact=i+1
                  goto 70
               elseif (ome .lt. xome(i+1)) then
                  iprev=i
                  ipost=i+1
                  goto 70
               endif
60          continue
70          continue
         endif
         if (iexact .gt. 0) then
            if (xk2 .le. xkap2(1)) then
               genspec=xspec(1,iexact)
            elseif (xk2 .eq. xkap2(npkap)) then
               genspec=xspec(npkap,iexact)
            elseif (xk2 .gt. xkap2(npkap)) then
               genspec=xspec(npkap,iexact)*
     >                 xkap2(npkap)*xkap2(npkap)/(xk2*xk2)
            else
               do 80 i=1, npkap, 1
                  aux1(i)=xspec(i,iexact)
80             continue
               genspec=ddivdif(aux1,xlkap2,npkap,dlog(xk2),4)
            endif
         else
            if (xk2 .le. xkap2(1)) then
               genprev=xspec(1,iprev)
               genpost=xspec(1,ipost)
            elseif (xk2 .eq. xkap2(npkap)) then
               genprev=xspec(npkap,iprev)
               genpost=xspec(npkap,ipost)
            elseif (xk2 .gt. xkap2(npkap)) then
               genprev=xspec(npkap,iprev)*
     >                 xkap2(npkap)*xkap2(npkap)/(xk2*xk2)
               genpost=xspec(npkap,ipost)*
     >                 xkap2(npkap)*xkap2(npkap)/(xk2*xk2)
            else
               do 90 i=1, npkap, 1
                  aux1(i)=xspec(i,iprev)
                  aux2(i)=xspec(i,ipost)
90             continue
               genprev=ddivdif(aux1,xlkap2,npkap,dlog(xk2),4)
               genpost=ddivdif(aux2,xlkap2,npkap,dlog(xk2),4)
            endif
            g12=genprev-genpost
            xl12=xlome(iprev)-xlome(ipost)
            c1=g12/xl12
            c2=genprev-c1*xlome(iprev)
            genspec=c1*dlog(ome)+c2
         endif
      endif
c
      RETURN
      END
C
*
* $Id: q-pythia.1.0.3.f,v 1.1 2013/06/24 12:41:00 yjlee Exp $
*
* $Log: q-pythia.1.0.3.f,v $
* Revision 1.1  2013/06/24 12:41:00  yjlee
* update
*
* Revision 1.1.1.1  1996/02/15 17:48:36  mclareni
* Kernlib
*
*
      FUNCTION DDIVDIF(F,A,NN,X,MM)
c     copy of cernlib divdif in double precision.
      implicit double precision (a-h,o-z)
      DIMENSION A(NN),F(NN),T(20),D(20)
      LOGICAL EXTRA
      LOGICAL MFLAG,RFLAG
      DATA MMAX/10/
C
C  TABULAR INTERPOLATION USING SYMMETRICALLY PLACED ARGUMENT POINTS.
C
C  START.  FIND SUBSCRIPT IX OF X IN ARRAY A.
C      IF( (NN.LT.2) .OR. (MM.LT.1) ) GO TO 20
      N=NN
      M=MIN0(MM,MMAX,N-1)
      MPLUS=M+1
      IX=0
      IY=N+1
      IF(A(1).GT.A(N)) GO TO 4
C     (SEARCH INCREASING ARGUMENTS.)
    1    MID=(IX+IY)/2
         IF(X.GE.A(MID)) GO TO 2
            IY=MID
            GO TO 3
C        (IF TRUE.)
    2       IX=MID
    3    IF(IY-IX.GT.1) GO TO 1
         GO TO 7
C     (SEARCH DECREASING ARGUMENTS.)
    4    MID=(IX+IY)/2
         IF(X.LE.A(MID)) GO TO 5
            IY=MID
            GO TO 6
C        (IF TRUE.)
    5       IX=MID
    6    IF(IY-IX.GT.1) GO TO 4
C
C  COPY REORDERED INTERPOLATION POINTS INTO (T(I),D(I)), SETTING
C  *EXTRA* TO TRUE IF M+2 POINTS TO BE USED.
    7 NPTS=M+2-MOD(M,2)
      IP=0
      L=0
      GO TO 9
    8    L=-L
         IF(L.GE.0) L=L+1
    9    ISUB=IX+L
         IF((1.LE.ISUB).AND.(ISUB.LE.N)) GO TO 10
C        (SKIP POINT.)
            NPTS=MPLUS
            GO TO 11
C        (INSERT POINT.)
   10       IP=IP+1
            T(IP)=A(ISUB)
            D(IP)=F(ISUB)
   11    IF(IP.LT.NPTS) GO TO 8
      EXTRA=NPTS.NE.MPLUS
C
C  REPLACE D BY THE LEADING DIAGONAL OF A DIVIDED-DIFFERENCE TABLE, SUP-
C  PLEMENTED BY AN EXTRA LINE IF *EXTRA* IS TRUE.
      DO 14 L=1,M
         IF(.NOT.EXTRA) GO TO 12
            ISUB=MPLUS-L
            D(M+2)=(D(M+2)-D(M))/(T(M+2)-T(ISUB))
   12    I=MPLUS
         DO 13 J=L,M
            ISUB=I-L
            D(I)=(D(I)-D(I-1))/(T(I)-T(ISUB))
            I=I-1
   13    CONTINUE
   14 CONTINUE
C
C  EVALUATE THE NEWTON INTERPOLATION FORMULA AT X, AVERAGING TWO VALUES
C  OF LAST DIFFERENCE IF *EXTRA* IS TRUE.
      SUM=D(MPLUS)
      IF(EXTRA) SUM=0.5*(SUM+D(M+2))
      J=M
      DO 15 L=1,M
         SUM=D(J)+(X-T(J))*SUM
         J=J-1
   15 CONTINUE
      DDIVDIF=SUM
      RETURN
C
C   20 CALL KERMTR('E105.1',LGFILE,MFLAG,RFLAG)
C      DDIVDIF=0
C      IF(MFLAG) THEN
C         IF(LGFILE.EQ.0) THEN
C            IF(MM.LT.1) WRITE(*,101) MM
C            IF(NN.LT.2) WRITE(*,102) NN
C         ELSE
C            IF(MM.LT.1) WRITE(LGFILE,101) MM
C            IF(NN.LT.2) WRITE(LGFILE,102) NN
C         ENDIF
C      ENDIF
C      IF(.NOT.RFLAG) CALL ABEND
C      RETURN
C  101 FORMAT( 7X, 'FUNCTION DDIVDIF ... M =',I6,' IS LESS THAN 1')
C  102 FORMAT( 7X, 'FUNCTION DDIVDIF ... N =',I6,' IS LESS THAN 2')
      END
c
C     COPY OF CERN DGAUSS
C
      FUNCTION DGAUSS1(F,A,B,EPS)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION W(12),X(12)
      PARAMETER (Z1 = 1.D0, HF = Z1/2.D0, CST = 5.D0*Z1/1000.D0)
      DATA X
     1        /0.96028 98564 97536 23168 35608 68569 47D0,
     2         0.79666 64774 13626 73959 15539 36475 83D0,
     3         0.52553 24099 16328 98581 77390 49189 25D0,
     4         0.18343 46424 95649 80493 94761 42360 18D0,
     5         0.98940 09349 91649 93259 61541 73450 33D0,
     6         0.94457 50230 73232 57607 79884 15534 61D0,
     7         0.86563 12023 87831 74388 04678 97712 39D0,
     8         0.75540 44083 55003 03389 51011 94847 44D0,
     9         0.61787 62444 02643 74844 66717 64048 79D0,
     A         0.45801 67776 57227 38634 24194 42983 58D0,
     B         0.28160 35507 79258 91323 04605 01460 50D0,
     C         0.95012 50983 76374 40185 31933 54249 58D-1/

      DATA W
     1        /0.10122 85362 90376 25915 25313 54309 96D0,
     2         0.22238 10344 53374 47054 43559 94426 24D0,
     3         0.31370 66458 77887 28733 79622 01986 60D0,
     4         0.36268 37833 78361 98296 51504 49277 20D0,
     5         0.27152 45941 17540 94851 78057 24560 18D-1,
     6         0.62253 52393 86478 92862 84383 69943 78D-1,
     7         0.95158 51168 24927 84809 92510 76022 46D-1,
     8         0.12462 89712 55533 87205 24762 82192 02D0,
     9         0.14959 59888 16576 73208 15017 30547 48D0,
     A         0.16915 65193 95002 53818 93120 79030 36D0,
     B         0.18260 34150 44923 58886 67636 67969 22D0,
     C         0.18945 06104 55068 49628 53967 23208 28D0/
      EXTERNAL F
      H=0.D0
      IF(B .EQ. A) GO TO 99
      CONST=CST/ABS(B-A)
      BB=A
    1 AA=BB
      BB=B
    2 C1=HF*(BB+AA)
      C2=HF*(BB-AA)
      S8=0.D0
      DO 3 I = 1,4
      U=C2*X(I)
    3 S8=S8+W(I)*(F(C1+U)+F(C1-U))
      S16=0.D0
      DO 4 I = 5,12
      U=C2*X(I)
    4 S16=S16+W(I)*(F(C1+U)+F(C1-U))
      S16=C2*S16
      IF(ABS(S16-C2*S8) .LE. EPS*(1.D0+ABS(S16))) THEN
       H=H+S16
       IF(BB .NE. B) GO TO 1
      ELSE
       BB=C1
       IF(1.D0+CONST*ABS(C2) .NE. 1.D0) GO TO 2
       H=0.D0
       WRITE(6,*) 'DGAUSS1: TOO HIGH ACCURACY REQUIRED'
       GO TO 99
      END IF
   99 DGAUSS1=H
      RETURN
      END
c
C     COPY OF CERN DGAUSS
C
      FUNCTION DGAUSS2(F,A,B,EPS)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION W(12),X(12)
      PARAMETER (Z1 = 1.D0, HF = Z1/2.D0, CST = 5.D0*Z1/1000.D0)
      DATA X
     1        /0.96028 98564 97536 23168 35608 68569 47D0,
     2         0.79666 64774 13626 73959 15539 36475 83D0,
     3         0.52553 24099 16328 98581 77390 49189 25D0,
     4         0.18343 46424 95649 80493 94761 42360 18D0,
     5         0.98940 09349 91649 93259 61541 73450 33D0,
     6         0.94457 50230 73232 57607 79884 15534 61D0,
     7         0.86563 12023 87831 74388 04678 97712 39D0,
     8         0.75540 44083 55003 03389 51011 94847 44D0,
     9         0.61787 62444 02643 74844 66717 64048 79D0,
     A         0.45801 67776 57227 38634 24194 42983 58D0,
     B         0.28160 35507 79258 91323 04605 01460 50D0,
     C         0.95012 50983 76374 40185 31933 54249 58D-1/

      DATA W
     1        /0.10122 85362 90376 25915 25313 54309 96D0,
     2         0.22238 10344 53374 47054 43559 94426 24D0,
     3         0.31370 66458 77887 28733 79622 01986 60D0,
     4         0.36268 37833 78361 98296 51504 49277 20D0,
     5         0.27152 45941 17540 94851 78057 24560 18D-1,
     6         0.62253 52393 86478 92862 84383 69943 78D-1,
     7         0.95158 51168 24927 84809 92510 76022 46D-1,
     8         0.12462 89712 55533 87205 24762 82192 02D0,
     9         0.14959 59888 16576 73208 15017 30547 48D0,
     A         0.16915 65193 95002 53818 93120 79030 36D0,
     B         0.18260 34150 44923 58886 67636 67969 22D0,
     C         0.18945 06104 55068 49628 53967 23208 28D0/
      EXTERNAL F
      H=0.D0
      IF(B .EQ. A) GO TO 99
      CONST=CST/ABS(B-A)
      BB=A
    1 AA=BB
      BB=B
    2 C1=HF*(BB+AA)
      C2=HF*(BB-AA)
      S8=0.D0
      DO 3 I = 1,4
      U=C2*X(I)
    3 S8=S8+W(I)*(F(C1+U)+F(C1-U))
      S16=0.D0
      DO 4 I = 5,12
      U=C2*X(I)
    4 S16=S16+W(I)*(F(C1+U)+F(C1-U))
      S16=C2*S16
      IF(ABS(S16-C2*S8) .LE. EPS*(1.D0+ABS(S16))) THEN
       H=H+S16
       IF(BB .NE. B) GO TO 1
      ELSE
       BB=C1
       IF(1.D0+CONST*ABS(C2) .NE. 1.D0) GO TO 2
       H=0.D0
       WRITE(6,*) 'DGAUSS2: TOO HIGH ACCURACY REQUIRED'
       GO TO 99
      END IF
   99 DGAUSS2=H
      RETURN
      END
c
      FUNCTION SIMDIS(Numb,zmin,nzur,RI)
C     IT SIMULATES A RANDOM NUMBER ACCORDING TO A DISCRETE DISTRIBUTION GIVEN
C     BY ARRAY YA AT POINTS XA. THOUGHT FOR PYTHIA (PYR(0)).
C     N: NUMBER OF POINTS IN THE ARRAYS.
C     XA: ARRAY OF X-VALUES.
C     YA: ARRAY OF Y-VALUES.
c     RI: VALUE OF THE INTEGRAL.
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION XA(500), YA(500)
      common/qpc1/eee,qhatl,omegac
      dlz=(1.d0-2.d0*zmin)/500.d0
      do 1 no=1,500
      xa(no)=zmin+no*dlz
      if(nzur.eq.1) ya(no)=splitq2(xa(no))
      if(nzur.eq.21) ya(no)=splitg2(xa(no))
      if(nzur.eq.3) ya(no)=splitqqbar(xa(no))
1     continue
      RAL=PYR(0)*RI
      XAUX=0.D0
      XAUXOLD=0.D0
      DO 10 I=2, Numb, 1
         XAUX=XAUX+(XA(I)-XA(I-1))*0.5D0*
     + (YA(I)+YA(I-1))
         IF (XAUX .GE. RAL) GOTO 20
         IF (I .EQ. Numb) THEN
            SIMDIS=XA(I)
            RETURN
         ENDIF
         XAUXOLD=XAUX
10    CONTINUE
20    SIMDIS=(XA(I)-XA(I-1))*(RAL-XAUXOLD)/(XAUX-XAUXOLD)+
     + XA(I-1)
      RETURN
      END
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     QPYSHOW ROUTINE FOR Q-PYTHIA version 1.0.2.
C     Change with respect to v1.0: no link with CERNLIB required.
C     Change with respect to v1.0.1: problem of multiple calls to
C        qpygin solved. Now the user must call routine qpygin0 once
C        per nucleon-nucleon collision and define there the initial
C        position and time in the cms of the collision.
C        Do not touch qpygin which automatically makes the boost and
C        rotation to the cms of the hard scattering.
C     Change with respect to v1.0.2: geometries by Konrad Tywoniuk added.
C
C     DATE: 10.03.2011.
C
C     AUTHORS: N. Armesto, L. Cunqueiro and C. A. Salgado
C              Departamento de Fisica de Particulas and IGFAE
C              Universidade de Santiago de Compostela
C              15706 Santiago de Compostela, Spain
C
C     EMAILS: nestor@fpaxp1.usc.es, leticia@fpaxp1.usc.es,
C             Carlos.Salgado@cern.ch
C
C     CONTENT: auxiliary files for modified QPYSHOW, fixed to PYTHIA-6.4.18.
C
C     WHEN USING Q-PYTHIA, PLEASE QUOTE:
C
C     1) N. Armesto, L. Cunqueiro and C. A. Salgado,
C        Eur. Phys. J. C63 (2009) 679 [arXiv:0907.1014 [hep-ph]].
C     2) T. Sjostrand, S. Mrenna and P. Skands,
C        ``PYTHIA 6.4 physics and manual,''
C        JHEP 0605 (2006) 026 [arXiv:hep-ph/0603175].
C
C     INSTRUCTIONS: initial parton position is initialized by a call
C                   to user-defined routine qpygin(x0,y0,z0,t0),
C                   where these are the initial coordinates in the
C                   center-of-mass frame of the hard collision
C                   (if applicable for the type of process you study). 
C                   The values of qhatL and omegac have to be computed
C                   by the user, using his preferred medium model, in
C                   routine qpygeo, which takes as input the position
C                   x,y,z,t of the parton to branch, the trajectory
C                   defined by the three-vector betax,betay,betaz,
C                   (all values in the center-of-mass frame of the
C                   hard collision), and  returns the value of qhatL
C                   (in GeV**2) and omegac (in GeV).
C                   Both routines are to be found at the end of this file.
C
C     DISCLAIMER: this program comes without any guarantees. Beware of
C                 errors and use common sense when interpreting results.
C                 Any modifications are done under exclusive
C                 maker's resposibility.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C*********************************************************************

C...QPYSHOW
C...Generates timelike parton showers from given partons.
 
      SUBROUTINE QPYSHOW(IP1,IP2,QMAX)
 
C...Double precision and integer declarations.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
C...Parameter statement to help give large particle numbers.
      PARAMETER (KSUSY1=1000000,KSUSY2=2000000,KTECHN=3000000,
     &KEXCIT=4000000,KDIMEN=5000000)
      PARAMETER (MAXNUR=1000)
Cacs+
      PARAMETER (NNPOS=4000)
      DIMENSION PPOS(NNPOS,4)
Cacs-
C...Commonblocks.
      COMMON/PYPART/NPART,NPARTD,IPART(MAXNUR),PTPART(MAXNUR)
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      SAVE /PYPART/,/PYJETS/,/PYDAT1/,/PYDAT2/,/PYPARS/,/PYINT1/
Cacs+
      common/qpc1/eee,qhatl,omegac     
      common/qpvir1/pmed
      common/qpvir2/virt
      COMMON/QPLT/QPLTA1,QPLTA2,QPLTBX,QPLTBY,QPLTBZ
      common/flcoh/iflcoh ! 1.0.3
      external splitg1
      external splitq1
      external splitg2
      external splitq2
      external splitqqbar
      data iflag/0/
Cacs-
C...Local arrays.
      DIMENSION PMTH(5,140),PS(5),PMA(100),PMSD(100),IEP(100),IPA(100),
     &KFLA(100),KFLD(100),KFL(100),ITRY(100),ISI(100),ISL(100),DP(100),
     &DPT(5,4),KSH(0:140),KCII(2),NIIS(2),IIIS(2,2),THEIIS(2,2),
     &PHIIIS(2,2),ISII(2),ISSET(2),ISCOL(0:140),ISCHG(0:140),
     &IREF(1000)
Cacs+
      IF (IFLAG .EQ. 0) THEN
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) '*******************************************'       
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) '            Q-PYTHIA version 1.0.2'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'DATE: 17.02.2010'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'AUTHORS: N. Armesto, L. Cunqueiro and'
         WRITE(MSTU(11),*) '         C. A. Salgado'
         WRITE(MSTU(11),*) ' Departamento de Fisica de Particulas'
         WRITE(MSTU(11),*) ' and IGFAE'
         WRITE(MSTU(11),*) ' Universidade de Santiago de Compostela'
         WRITE(MSTU(11),*) ' 15706 Santiago de Compostela, Spain'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'EMAILS: nestor@fpaxp1.usc.es,'
         WRITE(MSTU(11),*) '        leticia@fpaxp1.usc.es,' 
         WRITE(MSTU(11),*) '        Carlos.Salgado@cern.ch'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'NOTE: fixed to PYTHIA-6.4.18'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'WHEN USING Q-PYTHIA, PLEASE QUOTE:'
         WRITE(MSTU(11),*) '1) N. Armesto, L. Cunqueiro'
         WRITE(MSTU(11),*) '   and C. A. Salgado,'
         WRITE(MSTU(11),*) '   Eur. Phys. J. C63 (2009) 679'
         WRITE(MSTU(11),*) '   [arXiv:0907.1014 [hep-ph]].'
         WRITE(MSTU(11),*) '2) T. Sjostrand, S. Mrenna and P. Skands,'
         WRITE(MSTU(11),*) '   PYTHIA 6.4 physics and manual,'
         WRITE(MSTU(11),*) '   JHEP 0605 (2006) 026'
         WRITE(MSTU(11),*) '   [arXiv:hep-ph/0603175].'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'INSTRUCTIONS: look at the web page and'
         WRITE(MSTU(11),*) ' header of modfied routine QPYSHOW at the'
         WRITE(MSTU(11),*) ' end of Q-PYTHIA file.'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) 'DISCLAIMER: this program comes without any'
         WRITE(MSTU(11),*) ' guarantees. Beware of errors and use'
         WRITE(MSTU(11),*) ' common sense when interpreting results.'
         WRITE(MSTU(11),*) ' Any modifications are done under exclusive'
         WRITE(MSTU(11),*) ' makers resposibility.'
         WRITE(MSTU(11),*)
         WRITE(MSTU(11),*) '*******************************************'
         WRITE(MSTU(11),*)
         IFLAG=1
      ENDIF
Cacs-
 
C...Check that QMAX not too low.
      IF(MSTJ(41).LE.0) THEN
        RETURN
      ELSEIF(MSTJ(41).EQ.1.OR.MSTJ(41).EQ.11) THEN
        IF(QMAX.LE.PARJ(82).AND.IP2.GE.-80) RETURN
      ELSE
        IF(QMAX.LE.MIN(PARJ(82),PARJ(83),PARJ(90)).AND.IP2.GE.-80)
     &  RETURN
      ENDIF
 
C...Store positions of shower initiating partons.
      MPSPD=0
      IF(IP1.GT.0.AND.IP1.LE.MIN(N,MSTU(4)-MSTU(32)).AND.IP2.EQ.0) THEN
        NPA=1
        IPA(1)=IP1
      ELSEIF(MIN(IP1,IP2).GT.0.AND.MAX(IP1,IP2).LE.MIN(N,MSTU(4)-
     &  MSTU(32))) THEN
        NPA=2
        IPA(1)=IP1
        IPA(2)=IP2
      ELSEIF(IP1.GT.0.AND.IP1.LE.MIN(N,MSTU(4)-MSTU(32)).AND.IP2.LT.0
     &  .AND.IP2.GE.-80) THEN
        NPA=IABS(IP2)
        DO 100 I=1,NPA
          IPA(I)=IP1+I-1
  100   CONTINUE
      ELSEIF(IP1.GT.0.AND.IP1.LE.MIN(N,MSTU(4)-MSTU(32)).AND.
     &IP2.EQ.-100) THEN
        MPSPD=1
        NPA=2
        IPA(1)=IP1+6
        IPA(2)=IP1+7
      ELSE
        CALL PYERRM(12,
     &  '(QPYSHOW:) failed to reconstruct showering system')
        IF(MSTU(21).GE.1) RETURN
      ENDIF
 
C...Send off to PYPTFS for pT-ordered evolution if requested,
C...if at least 2 partons, and without predefined shower branchings.
      IF((MSTJ(41).EQ.11.OR.MSTJ(41).EQ.12).AND.NPA.GE.2.AND.
     &MPSPD.EQ.0) THEN
        NPART=NPA
        DO 110 II=1,NPART
          IPART(II)=IPA(II)
          PTPART(II)=0.5D0*QMAX
  110   CONTINUE
        CALL PYPTFS(2,0.5D0*QMAX,0D0,PTGEN)
        RETURN
      ENDIF
 
C...Initialization of cutoff masses etc.
      DO 120 IFL=0,40
        ISCOL(IFL)=0
        ISCHG(IFL)=0
        KSH(IFL)=0
  120 CONTINUE
      ISCOL(21)=1
      KSH(21)=1
      PMTH(1,21)=PYMASS(21)
      PMTH(2,21)=SQRT(PMTH(1,21)**2+0.25D0*PARJ(82)**2)
      PMTH(3,21)=2D0*PMTH(2,21)
      PMTH(4,21)=PMTH(3,21)
      PMTH(5,21)=PMTH(3,21)
      PMTH(1,22)=PYMASS(22)
      PMTH(2,22)=SQRT(PMTH(1,22)**2+0.25D0*PARJ(83)**2)
      PMTH(3,22)=2D0*PMTH(2,22)
      PMTH(4,22)=PMTH(3,22)
      PMTH(5,22)=PMTH(3,22)
      PMQTH1=PARJ(82)
      IF(MSTJ(41).GE.2) PMQTH1=MIN(PARJ(82),PARJ(83))
      PMQT1E=MIN(PMQTH1,PARJ(90))
      PMQTH2=PMTH(2,21)
      IF(MSTJ(41).GE.2) PMQTH2=MIN(PMTH(2,21),PMTH(2,22))
      PMQT2E=MIN(PMQTH2,0.5D0*PARJ(90))
      DO 130 IFL=1,5
        ISCOL(IFL)=1
        IF(MSTJ(41).GE.2) ISCHG(IFL)=1
        KSH(IFL)=1
        PMTH(1,IFL)=PYMASS(IFL)
        PMTH(2,IFL)=SQRT(PMTH(1,IFL)**2+0.25D0*PMQTH1**2)
        PMTH(3,IFL)=PMTH(2,IFL)+PMQTH2
        PMTH(4,IFL)=SQRT(PMTH(1,IFL)**2+0.25D0*PARJ(82)**2)+PMTH(2,21)
        PMTH(5,IFL)=SQRT(PMTH(1,IFL)**2+0.25D0*PARJ(83)**2)+PMTH(2,22)
  130 CONTINUE
      DO 140 IFL=11,15,2
        IF(MSTJ(41).EQ.2.OR.MSTJ(41).GE.4) ISCHG(IFL)=1
        IF(MSTJ(41).EQ.2.OR.MSTJ(41).GE.4) KSH(IFL)=1
        PMTH(1,IFL)=PYMASS(IFL)
        PMTH(2,IFL)=SQRT(PMTH(1,IFL)**2+0.25D0*PARJ(90)**2)
        PMTH(3,IFL)=PMTH(2,IFL)+0.5D0*PARJ(90)
        PMTH(4,IFL)=PMTH(3,IFL)
        PMTH(5,IFL)=PMTH(3,IFL)
  140 CONTINUE
      PT2MIN=MAX(0.5D0*PARJ(82),1.1D0*PARJ(81))**2
      ALAMS=PARJ(81)**2
      ALFM=LOG(PT2MIN/ALAMS)
 
C...Check on phase space available for emission.
      IREJ=0
      DO 150 J=1,5
        PS(J)=0D0
  150 CONTINUE
      PM=0D0
      KFLA(2)=0
      DO 170 I=1,NPA
        KFLA(I)=IABS(K(IPA(I),2))
        PMA(I)=P(IPA(I),5)
C...Special cutoff masses for initial partons (may be a heavy quark,
C...squark, ..., and need not be on the mass shell).
        IR=30+I
        IF(NPA.LE.1) IREF(I)=IR
        IF(NPA.GE.2) IREF(I+1)=IR
        ISCOL(IR)=0
        ISCHG(IR)=0
        KSH(IR)=0
        IF(KFLA(I).LE.8) THEN
          ISCOL(IR)=1
          IF(MSTJ(41).GE.2) ISCHG(IR)=1
        ELSEIF(KFLA(I).EQ.11.OR.KFLA(I).EQ.13.OR.KFLA(I).EQ.15.OR.
     &  KFLA(I).EQ.17) THEN
          IF(MSTJ(41).EQ.2.OR.MSTJ(41).GE.4) ISCHG(IR)=1
        ELSEIF(KFLA(I).EQ.21) THEN
          ISCOL(IR)=1
        ELSEIF((KFLA(I).GE.KSUSY1+1.AND.KFLA(I).LE.KSUSY1+8).OR.
     &  (KFLA(I).GE.KSUSY2+1.AND.KFLA(I).LE.KSUSY2+8)) THEN
          ISCOL(IR)=1
        ELSEIF(KFLA(I).EQ.KSUSY1+21) THEN
          ISCOL(IR)=1
C...QUARKONIA+++
C...same for QQ~[3S18]
        ELSEIF(MSTP(148).GE.1.AND.(KFLA(I).EQ.9900443.OR.
     &  KFLA(I).EQ.9900553)) THEN
          ISCOL(IR)=1
C...QUARKONIA---
        ENDIF
        IF(ISCOL(IR).EQ.1.OR.ISCHG(IR).EQ.1) KSH(IR)=1
        PMTH(1,IR)=PMA(I)
        IF(ISCOL(IR).EQ.1.AND.ISCHG(IR).EQ.1) THEN
          PMTH(2,IR)=SQRT(PMTH(1,IR)**2+0.25D0*PMQTH1**2)
          PMTH(3,IR)=PMTH(2,IR)+PMQTH2
          PMTH(4,IR)=SQRT(PMTH(1,IR)**2+0.25D0*PARJ(82)**2)+PMTH(2,21)
          PMTH(5,IR)=SQRT(PMTH(1,IR)**2+0.25D0*PARJ(83)**2)+PMTH(2,22)
        ELSEIF(ISCOL(IR).EQ.1) THEN
          PMTH(2,IR)=SQRT(PMTH(1,IR)**2+0.25D0*PARJ(82)**2)
          PMTH(3,IR)=PMTH(2,IR)+0.5D0*PARJ(82)
          PMTH(4,IR)=PMTH(3,IR)
          PMTH(5,IR)=PMTH(3,IR)
        ELSEIF(ISCHG(IR).EQ.1) THEN
          PMTH(2,IR)=SQRT(PMTH(1,IR)**2+0.25D0*PARJ(90)**2)
          PMTH(3,IR)=PMTH(2,IR)+0.5D0*PARJ(90)
          PMTH(4,IR)=PMTH(3,IR)
          PMTH(5,IR)=PMTH(3,IR)
        ENDIF
        IF(KSH(IR).EQ.1) PMA(I)=PMTH(3,IR)
        PM=PM+PMA(I)
        IF(KSH(IR).EQ.0.OR.PMA(I).GT.10D0*QMAX) IREJ=IREJ+1
        DO 160 J=1,4
          PS(J)=PS(J)+P(IPA(I),J)
  160   CONTINUE
  170 CONTINUE
      IF(IREJ.EQ.NPA.AND.IP2.GE.-7) RETURN
      PS(5)=SQRT(MAX(0D0,PS(4)**2-PS(1)**2-PS(2)**2-PS(3)**2))
      IF(NPA.EQ.1) PS(5)=PS(4)
      IF(PS(5).LE.PM+PMQT1E) RETURN
 
C...Identify source: q(1), ~q(2), V(3), S(4), chi(5), ~g(6), unknown(0).
      KFSRCE=0
      IF(IP2.LE.0) THEN
      ELSEIF(K(IP1,3).EQ.K(IP2,3).AND.K(IP1,3).GT.0) THEN
        KFSRCE=IABS(K(K(IP1,3),2))
      ELSE
        IPAR1=MAX(1,K(IP1,3))
        IPAR2=MAX(1,K(IP2,3))
        IF(K(IPAR1,3).EQ.K(IPAR2,3).AND.K(IPAR1,3).GT.0)
     &       KFSRCE=IABS(K(K(IPAR1,3),2))
      ENDIF
      ITYPES=0
      IF(KFSRCE.GE.1.AND.KFSRCE.LE.8) ITYPES=1
      IF(KFSRCE.GE.KSUSY1+1.AND.KFSRCE.LE.KSUSY1+8) ITYPES=2
      IF(KFSRCE.GE.KSUSY2+1.AND.KFSRCE.LE.KSUSY2+8) ITYPES=2
      IF(KFSRCE.GE.21.AND.KFSRCE.LE.24) ITYPES=3
      IF(KFSRCE.GE.32.AND.KFSRCE.LE.34) ITYPES=3
      IF(KFSRCE.EQ.25.OR.(KFSRCE.GE.35.AND.KFSRCE.LE.37)) ITYPES=4
      IF(KFSRCE.GE.KSUSY1+22.AND.KFSRCE.LE.KSUSY1+37) ITYPES=5
      IF(KFSRCE.EQ.KSUSY1+21) ITYPES=6
 
C...Identify two primary showerers.
      ITYPE1=0
      IF(KFLA(1).GE.1.AND.KFLA(1).LE.8) ITYPE1=1
      IF(KFLA(1).GE.KSUSY1+1.AND.KFLA(1).LE.KSUSY1+8) ITYPE1=2
      IF(KFLA(1).GE.KSUSY2+1.AND.KFLA(1).LE.KSUSY2+8) ITYPE1=2
      IF(KFLA(1).GE.21.AND.KFLA(1).LE.24) ITYPE1=3
      IF(KFLA(1).GE.32.AND.KFLA(1).LE.34) ITYPE1=3
      IF(KFLA(1).EQ.25.OR.(KFLA(1).GE.35.AND.KFLA(1).LE.37)) ITYPE1=4
      IF(KFLA(1).GE.KSUSY1+22.AND.KFLA(1).LE.KSUSY1+37) ITYPE1=5
      IF(KFLA(1).EQ.KSUSY1+21) ITYPE1=6
      ITYPE2=0
      IF(KFLA(2).GE.1.AND.KFLA(2).LE.8) ITYPE2=1
      IF(KFLA(2).GE.KSUSY1+1.AND.KFLA(2).LE.KSUSY1+8) ITYPE2=2
      IF(KFLA(2).GE.KSUSY2+1.AND.KFLA(2).LE.KSUSY2+8) ITYPE2=2
      IF(KFLA(2).GE.21.AND.KFLA(2).LE.24) ITYPE2=3
      IF(KFLA(2).GE.32.AND.KFLA(2).LE.34) ITYPE2=3
      IF(KFLA(2).EQ.25.OR.(KFLA(2).GE.35.AND.KFLA(2).LE.37)) ITYPE2=4
      IF(KFLA(2).GE.KSUSY1+22.AND.KFLA(2).LE.KSUSY1+37) ITYPE2=5
      IF(KFLA(2).EQ.KSUSY1+21) ITYPE2=6
 
C...Order of showerers. Presence of gluino.
      ITYPMN=MIN(ITYPE1,ITYPE2)
      ITYPMX=MAX(ITYPE1,ITYPE2)
      IORD=1
      IF(ITYPE1.GT.ITYPE2) IORD=2
      IGLUI=0
      IF(ITYPE1.EQ.6.OR.ITYPE2.EQ.6) IGLUI=1
 
C...Check if 3-jet matrix elements to be used.
      M3JC=0
      ALPHA=0.5D0
      IF(NPA.EQ.2.AND.MSTJ(47).GE.1.AND.MPSPD.EQ.0) THEN
        IF(MSTJ(38).NE.0) THEN
          M3JC=MSTJ(38)
          ALPHA=PARJ(80)
          MSTJ(38)=0
        ELSEIF(MSTJ(47).GE.6) THEN
          M3JC=MSTJ(47)
        ELSE
          ICLASS=1
          ICOMBI=4
 
C...Vector/axial vector -> q + qbar; q -> q + V.
          IF(ITYPMN.EQ.1.AND.ITYPMX.EQ.1.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.3)) THEN
            ICLASS=2
            IF(KFSRCE.EQ.21.OR.KFSRCE.EQ.22) THEN
              ICOMBI=1
            ELSEIF(KFSRCE.EQ.23.OR.(KFSRCE.EQ.0.AND.
     &      K(IPA(1),2)+K(IPA(2),2).EQ.0)) THEN
C...gamma*/Z0: assume e+e- initial state if unknown.
              EI=-1D0
              IF(KFSRCE.EQ.23) THEN
                IANNFL=K(K(IP1,3),3)
                IF(IANNFL.NE.0) THEN
                  KANNFL=IABS(K(IANNFL,2))
                  IF(KANNFL.GE.1.AND.KANNFL.LE.18) EI=KCHG(KANNFL,1)/3D0
                ENDIF
              ENDIF
              AI=SIGN(1D0,EI+0.1D0)
              VI=AI-4D0*EI*PARU(102)
              EF=KCHG(KFLA(1),1)/3D0
              AF=SIGN(1D0,EF+0.1D0)
              VF=AF-4D0*EF*PARU(102)
              XWC=1D0/(16D0*PARU(102)*(1D0-PARU(102)))
              SH=PS(5)**2
              SQMZ=PMAS(23,1)**2
              SQWZ=PS(5)*PMAS(23,2)
              SBWZ=1D0/((SH-SQMZ)**2+SQWZ**2)
              VECT=EI**2*EF**2+2D0*EI*VI*EF*VF*XWC*SH*(SH-SQMZ)*SBWZ+
     &        (VI**2+AI**2)*VF**2*XWC**2*SH**2*SBWZ
              AXIV=(VI**2+AI**2)*AF**2*XWC**2*SH**2*SBWZ
              ICOMBI=3
              ALPHA=VECT/(VECT+AXIV)
            ELSEIF(KFSRCE.EQ.24.OR.KFSRCE.EQ.0) THEN
              ICOMBI=4
            ENDIF
C...For chi -> chi q qbar, use V/A -> q qbar as first approximation.
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.1.AND.ITYPES.EQ.5) THEN
            ICLASS=2
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.3.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.1)) THEN
            ICLASS=3
 
C...Scalar/pseudoscalar -> q + qbar; q -> q + S.
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.1.AND.ITYPES.EQ.4) THEN
            ICLASS=4
            IF(KFSRCE.EQ.25.OR.KFSRCE.EQ.35.OR.KFSRCE.EQ.37) THEN
              ICOMBI=1
            ELSEIF(KFSRCE.EQ.36) THEN
              ICOMBI=2
            ENDIF
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.4.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.1)) THEN
            ICLASS=5
 
C...V -> ~q + ~qbar; ~q -> ~q + V; S -> ~q + ~qbar; ~q -> ~q + S.
          ELSEIF(ITYPMN.EQ.2.AND.ITYPMX.EQ.2.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.3)) THEN
            ICLASS=6
          ELSEIF(ITYPMN.EQ.2.AND.ITYPMX.EQ.3.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.2)) THEN
            ICLASS=7
          ELSEIF(ITYPMN.EQ.2.AND.ITYPMX.EQ.2.AND.ITYPES.EQ.4) THEN
            ICLASS=8
          ELSEIF(ITYPMN.EQ.2.AND.ITYPMX.EQ.4.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.2)) THEN
            ICLASS=9
 
C...chi -> q + ~qbar; ~q -> q + chi; q -> ~q + chi.
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.2.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.5)) THEN
            ICLASS=10
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.5.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.2)) THEN
            ICLASS=11
          ELSEIF(ITYPMN.EQ.2.AND.ITYPMX.EQ.5.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.1)) THEN
            ICLASS=12
 
C...~g -> q + ~qbar; ~q -> q + ~g; q -> ~q + ~g.
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.2.AND.ITYPES.EQ.6) THEN
            ICLASS=13
          ELSEIF(ITYPMN.EQ.1.AND.ITYPMX.EQ.6.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.2)) THEN
            ICLASS=14
          ELSEIF(ITYPMN.EQ.2.AND.ITYPMX.EQ.6.AND.(ITYPES.EQ.0.OR.
     &    ITYPES.EQ.1)) THEN
            ICLASS=15
 
C...g -> ~g + ~g (eikonal approximation).
          ELSEIF(ITYPMN.EQ.6.AND.ITYPMX.EQ.6.AND.ITYPES.EQ.0) THEN
            ICLASS=16
          ENDIF
          M3JC=5*ICLASS+ICOMBI
        ENDIF
      ENDIF
 
C...Find if interference with initial state partons.
      MIIS=0
      IF(MSTJ(50).GE.1.AND.MSTJ(50).LE.3.AND.NPA.EQ.2.AND.KFSRCE.EQ.0
     &.AND.MPSPD.EQ.0) MIIS=MSTJ(50)
      IF(MSTJ(50).GE.4.AND.MSTJ(50).LE.6.AND.NPA.EQ.2.AND.MPSPD.EQ.0)
     &MIIS=MSTJ(50)-3
      IF(MIIS.NE.0) THEN
        DO 190 I=1,2
          KCII(I)=0
          KCA=PYCOMP(KFLA(I))
          IF(KCA.NE.0) KCII(I)=KCHG(KCA,2)*ISIGN(1,K(IPA(I),2))
          NIIS(I)=0
          IF(KCII(I).NE.0) THEN
            DO 180 J=1,2
              ICSI=MOD(K(IPA(I),3+J)/MSTU(5),MSTU(5))
              IF(ICSI.GT.0.AND.ICSI.NE.IPA(1).AND.ICSI.NE.IPA(2).AND.
     &        (KCII(I).EQ.(-1)**(J+1).OR.KCII(I).EQ.2)) THEN
                NIIS(I)=NIIS(I)+1
                IIIS(I,NIIS(I))=ICSI
              ENDIF
  180       CONTINUE
          ENDIF
  190   CONTINUE
        IF(NIIS(1)+NIIS(2).EQ.0) MIIS=0
      ENDIF
 
C...Boost interfering initial partons to rest frame
C...and reconstruct their polar and azimuthal angles.
Cacs+
        qplta1=0.d0
        qplta2=0.d0
        qpltbx=0.d0
        qpltby=0.d0
        qpltbz=0.d0
Cacs-
      IF(MIIS.NE.0) THEN
        DO 210 I=1,2
          DO 200 J=1,5
            K(N+I,J)=K(IPA(I),J)
            P(N+I,J)=P(IPA(I),J)
            V(N+I,J)=0D0
  200     CONTINUE
  210   CONTINUE
        DO 230 I=3,2+NIIS(1)
          DO 220 J=1,5
            K(N+I,J)=K(IIIS(1,I-2),J)
            P(N+I,J)=P(IIIS(1,I-2),J)
            V(N+I,J)=0D0
  220     CONTINUE
  230   CONTINUE
        DO 250 I=3+NIIS(1),2+NIIS(1)+NIIS(2)
          DO 240 J=1,5
            K(N+I,J)=K(IIIS(2,I-2-NIIS(1)),J)
            P(N+I,J)=P(IIIS(2,I-2-NIIS(1)),J)
            V(N+I,J)=0D0
  240     CONTINUE
  250   CONTINUE
        CALL PYROBO(N+1,N+2+NIIS(1)+NIIS(2),0D0,0D0,-PS(1)/PS(4),
     &  -PS(2)/PS(4),-PS(3)/PS(4))
        PHI=PYANGL(P(N+1,1),P(N+1,2))
        CALL PYROBO(N+1,N+2+NIIS(1)+NIIS(2),0D0,-PHI,0D0,0D0,0D0)
        THE=PYANGL(P(N+1,3),P(N+1,1))
        CALL PYROBO(N+1,N+2+NIIS(1)+NIIS(2),-THE,0D0,0D0,0D0,0D0)
Cacs+
        qplta1=-the
        qplta2=-phi
        qpltbx=-PS(1)/PS(4)
        qpltby=-PS(2)/PS(4)
        qpltbz=-PS(3)/PS(4)
Cacs-
        DO 260 I=3,2+NIIS(1)
          THEIIS(1,I-2)=PYANGL(P(N+I,3),SQRT(P(N+I,1)**2+P(N+I,2)**2))
          PHIIIS(1,I-2)=PYANGL(P(N+I,1),P(N+I,2))
  260   CONTINUE
        DO 270 I=3+NIIS(1),2+NIIS(1)+NIIS(2)
          THEIIS(2,I-2-NIIS(1))=PARU(1)-PYANGL(P(N+I,3),
     &    SQRT(P(N+I,1)**2+P(N+I,2)**2))
          PHIIIS(2,I-2-NIIS(1))=PYANGL(P(N+I,1),P(N+I,2))
  270   CONTINUE
      ENDIF
 
C...Boost 3 or more partons to their rest frame.
Cacs+
c      IF(NPA.GE.3) CALL PYROBO(IPA(1),IPA(NPA),0D0,0D0,-PS(1)/PS(4),
c     &-PS(2)/PS(4),-PS(3)/PS(4))
      IF(NPA.GE.3) THEN
        CALL PYROBO(IPA(1),IPA(NPA),0D0,0D0,-PS(1)/PS(4),
     &-PS(2)/PS(4),-PS(3)/PS(4))
        qplta1=0.d0
        qplta2=0.d0
        qpltbx=-PS(1)/PS(4)
        qpltby=-PS(2)/PS(4)
        qpltbz=-PS(3)/PS(4)
      ENDIF
Cacs-
 
C...Define imagined single initiator of shower for parton system.
      NS=N
      IF(N.GT.MSTU(4)-MSTU(32)-10) THEN
        CALL PYERRM(11,'(QPYSHOW:) no more memory left in PYJETS')
        IF(MSTU(21).GE.1) RETURN
      ENDIF
  280 N=NS
      IF(NPA.GE.2) THEN
        K(N+1,1)=11
        K(N+1,2)=21
        K(N+1,3)=0
        K(N+1,4)=0
        K(N+1,5)=0
        P(N+1,1)=0D0
        P(N+1,2)=0D0
        P(N+1,3)=0D0
        P(N+1,4)=PS(5)
        P(N+1,5)=PS(5)
        V(N+1,5)=PS(5)**2
        N=N+1
        IREF(1)=21
      ENDIF
Cacs+
      call qpygin(pposx0,pposy0,pposz0,ppost0) ! in fm
      do 10101 iijj=1, nnpos, 1
         ppos(iijj,1)=pposx0
         ppos(iijj,2)=pposy0
         ppos(iijj,3)=pposz0
         ppos(iijj,4)=ppost0
10101 continue
Cacs-
 
C...Loop over partons that may branch.
      NEP=NPA
      IM=NS
      IF(NPA.EQ.1) IM=NS-1
  290 IM=IM+1
      IF(N.GT.NS) THEN
        IF(IM.GT.N) GOTO 600
        KFLM=IABS(K(IM,2))
        IR=IREF(IM-NS)
        IF(KSH(IR).EQ.0) GOTO 290
        IF(P(IM,5).LT.PMTH(2,IR)) GOTO 290
        IGM=K(IM,3)
      ELSE
        IGM=-1
      ENDIF
      IF(N+NEP.GT.MSTU(4)-MSTU(32)-10) THEN
        CALL PYERRM(11,'(QPYSHOW:) no more memory left in PYJETS')
        IF(MSTU(21).GE.1) RETURN
      ENDIF
 
C...Position of aunt (sister to branching parton).
C...Origin and flavour of daughters.
      IAU=0
      IF(IGM.GT.0) THEN
        IF(K(IM-1,3).EQ.IGM) IAU=IM-1
        IF(N.GE.IM+1.AND.K(IM+1,3).EQ.IGM) IAU=IM+1
      ENDIF
      IF(IGM.GE.0) THEN
        K(IM,4)=N+1
        DO 300 I=1,NEP
          K(N+I,3)=IM
  300   CONTINUE
      ELSE
        K(N+1,3)=IPA(1)
      ENDIF
      IF(IGM.LE.0) THEN
        DO 310 I=1,NEP
          K(N+I,2)=K(IPA(I),2)
  310   CONTINUE
      ELSEIF(KFLM.NE.21) THEN
        K(N+1,2)=K(IM,2)
        K(N+2,2)=K(IM,5)
        IREF(N+1-NS)=IREF(IM-NS)
        IREF(N+2-NS)=IABS(K(N+2,2))
      ELSEIF(K(IM,5).EQ.21) THEN
        K(N+1,2)=21
        K(N+2,2)=21
        IREF(N+1-NS)=21
        IREF(N+2-NS)=21
      ELSE
        K(N+1,2)=K(IM,5)
        K(N+2,2)=-K(IM,5)
        IREF(N+1-NS)=IABS(K(N+1,2))
        IREF(N+2-NS)=IABS(K(N+2,2))
      ENDIF
 
C...Reset flags on daughters and tries made.
      DO 320 IP=1,NEP
        K(N+IP,1)=3
        K(N+IP,4)=0
        K(N+IP,5)=0
        KFLD(IP)=IABS(K(N+IP,2))
        IF(KCHG(PYCOMP(KFLD(IP)),2).EQ.0) K(N+IP,1)=1
        ITRY(IP)=0
        ISL(IP)=0
        ISI(IP)=0
        IF(KSH(IREF(N+IP-NS)).EQ.1) ISI(IP)=1
  320 CONTINUE
      ISLM=0
 
C...Maximum virtuality of daughters.
      IF(IGM.LE.0) THEN
        DO 330 I=1,NPA
          IF(NPA.GE.3) P(N+I,4)=P(IPA(I),4)
          P(N+I,5)=MIN(QMAX,PS(5))
          IR=IREF(N+I-NS)
          IF(IP2.LE.-8) P(N+I,5)=MAX(P(N+I,5),2D0*PMTH(3,IR))
          IF(ISI(I).EQ.0) P(N+I,5)=P(IPA(I),5)
  330   CONTINUE
      ELSE
        IF(MSTJ(43).LE.2) PEM=V(IM,2)
        IF(MSTJ(43).GE.3) PEM=P(IM,4)
        P(N+1,5)=MIN(P(IM,5),V(IM,1)*PEM)
        P(N+2,5)=MIN(P(IM,5),(1D0-V(IM,1))*PEM)
        IF(K(N+2,2).EQ.22) P(N+2,5)=PMTH(1,22)
      ENDIF
      DO 340 I=1,NEP
        PMSD(I)=P(N+I,5)
        IF(ISI(I).EQ.1) THEN
          IR=IREF(N+I-NS)
          IF(P(N+I,5).LE.PMTH(3,IR)) P(N+I,5)=PMTH(1,IR)
        ENDIF
        V(N+I,5)=P(N+I,5)**2
  340 CONTINUE
 
C...Choose one of the daughters for evolution.
  350 INUM=0
      IF(NEP.EQ.1) INUM=1
      DO 360 I=1,NEP
        IF(INUM.EQ.0.AND.ISL(I).EQ.1) INUM=I
  360 CONTINUE
      DO 370 I=1,NEP
        IF(INUM.EQ.0.AND.ITRY(I).EQ.0.AND.ISI(I).EQ.1) THEN
          IR=IREF(N+I-NS)
          IF(P(N+I,5).GE.PMTH(2,IR)) INUM=I
        ENDIF
  370 CONTINUE
      IF(INUM.EQ.0) THEN
        RMAX=0D0
        DO 380 I=1,NEP
          IF(ISI(I).EQ.1.AND.PMSD(I).GE.PMQT2E) THEN
            RPM=P(N+I,5)/PMSD(I)
            IR=IREF(N+I-NS)
            IF(RPM.GT.RMAX.AND.P(N+I,5).GE.PMTH(2,IR)) THEN
              RMAX=RPM
              INUM=I
            ENDIF
          ENDIF
  380   CONTINUE
      ENDIF
 
C...Cancel choice of predetermined daughter already treated.
      INUM=MAX(1,INUM)
      INUMT=INUM
      IF(MPSPD.EQ.1.AND.IGM.EQ.0.AND.ITRY(INUMT).GE.1) THEN
        IF(K(IP1-1+INUM,4).GT.0) INUM=3-INUM
      ELSEIF(MPSPD.EQ.1.AND.IM.EQ.NS+2.AND.ITRY(INUMT).GE.1) THEN
        IF(KFLD(INUMT).NE.21.AND.K(IP1+2,4).GT.0) INUM=3-INUM
        IF(KFLD(INUMT).EQ.21.AND.K(IP1+3,4).GT.0) INUM=3-INUM
      ENDIF
 
C...Store information on choice of evolving daughter.
      IEP(1)=N+INUM
Cacs+
      idf=k(iep(1),3)
      zz1=v(idf,1)
      zzz=zz1
      zz2=1.d0-zz1
      if (nep .gt. 1 .and. inum .eq. 2) then
         zzz=zz2
      endif        
      ttt=v(idf,5)
      if(zz1.gt.0.d0) then
            eee=zzz*p(idf,4)
      else
            eee=p(idf,4)
      endif
      xkt=zz1*zz2*ttt
      if ((xkt .gt. 0.d0) .and. (iflcoh .eq. 1)) then ! 1.0.3
         xlcoh=(2.d0*eee/(zz1*zz2*ttt))*0.1973d0
      else
         xlcoh=0.d0
      endif      
      if (idf .eq. 0) then ! for the initial parton if it has no father
         xbx=p(iep(1),1)/p(iep(1),4)
         xby=p(iep(1),2)/p(iep(1),4)
         xbz=p(iep(1),3)/p(iep(1),4)
         call qpygeo(pposx0,pposy0,pposz0,ppost0,
     >               xbx,xby,xbz,qhatl,omegac)
      else
         xbx=p(idf,1)/p(idf,4)
         xby=p(idf,2)/p(idf,4)
         xbz=p(idf,3)/p(idf,4)
         ppos(iep(1),1)=ppos(idf,1)+xbx*xlcoh
         ppos(iep(1),2)=ppos(idf,2)+xby*xlcoh
         ppos(iep(1),3)=ppos(idf,3)+xbz*xlcoh
         ppos(iep(1),4)=ppos(idf,4)+xlcoh
         call qpygeo(ppos(iep(1),1),ppos(iep(1),2),ppos(iep(1),3),
     >               ppos(iep(1),4),xbx,xby,xbz,qhatl,omegac)
      endif
Cacs-
      DO 390 I=2,NEP
        IEP(I)=IEP(I-1)+1
        IF(IEP(I).GT.N+NEP) IEP(I)=N+1
  390 CONTINUE
      DO 400 I=1,NEP
        KFL(I)=IABS(K(IEP(I),2))
  400 CONTINUE
      ITRY(INUM)=ITRY(INUM)+1
      IF(ITRY(INUM).GT.200) THEN
        CALL PYERRM(14,'(QPYSHOW:) caught in infinite loop')
        IF(MSTU(21).GE.1) RETURN
      ENDIF
      Z=0.5D0
      IR=IREF(IEP(1)-NS)
      IF(KSH(IR).EQ.0) GOTO 450
      IF(P(IEP(1),5).LT.PMTH(2,IR)) GOTO 450
 
C...Check if evolution already predetermined for daughter.
      IPSPD=0
      IF(MPSPD.EQ.1.AND.IGM.EQ.0) THEN
        IF(K(IP1-1+INUM,4).GT.0) IPSPD=IP1-1+INUM
      ELSEIF(MPSPD.EQ.1.AND.IM.EQ.NS+2) THEN
        IF(KFL(1).NE.21.AND.K(IP1+2,4).GT.0) IPSPD=IP1+2
        IF(KFL(1).EQ.21.AND.K(IP1+3,4).GT.0) IPSPD=IP1+3
      ENDIF
      IF(INUM.EQ.1.OR.INUM.EQ.2) THEN
        ISSET(INUM)=0
        IF(IPSPD.NE.0) ISSET(INUM)=1
      ENDIF
 
C...Select side for interference with initial state partons.
      IF(MIIS.GE.1.AND.IEP(1).LE.NS+3) THEN
        III=IEP(1)-NS-1
        ISII(III)=0
        IF(IABS(KCII(III)).EQ.1.AND.NIIS(III).EQ.1) THEN
          ISII(III)=1
        ELSEIF(KCII(III).EQ.2.AND.NIIS(III).EQ.1) THEN
          IF(PYR(0).GT.0.5D0) ISII(III)=1
        ELSEIF(KCII(III).EQ.2.AND.NIIS(III).EQ.2) THEN
          ISII(III)=1
          IF(PYR(0).GT.0.5D0) ISII(III)=2
        ENDIF
      ENDIF
 
C...Calculate allowed z range.
      IF(NEP.EQ.1) THEN
        PMED=PS(4)
      ELSEIF(IGM.EQ.0.OR.MSTJ(43).LE.2) THEN
        PMED=P(IM,5)
      ELSE
        IF(INUM.EQ.1) PMED=V(IM,1)*PEM
        IF(INUM.EQ.2) PMED=(1D0-V(IM,1))*PEM
      ENDIF
      IF(MOD(MSTJ(43),2).EQ.1) THEN
        ZC=PMTH(2,21)/PMED
        ZCE=PMTH(2,22)/PMED
        IF(ISCOL(IR).EQ.0) ZCE=0.5D0*PARJ(90)/PMED
      ELSE
        ZC=0.5D0*(1D0-SQRT(MAX(0D0,1D0-(2D0*PMTH(2,21)/PMED)**2)))
        IF(ZC.LT.1D-6) ZC=(PMTH(2,21)/PMED)**2
        PMTMPE=PMTH(2,22)
        IF(ISCOL(IR).EQ.0) PMTMPE=0.5D0*PARJ(90)
        ZCE=0.5D0*(1D0-SQRT(MAX(0D0,1D0-(2D0*PMTMPE/PMED)**2)))
        IF(ZCE.LT.1D-6) ZCE=(PMTMPE/PMED)**2
      ENDIF
      ZC=MIN(ZC,0.491D0)
      ZCE=MIN(ZCE,0.49991D0)
      IF(((MSTJ(41).EQ.1.AND.ZC.GT.0.49D0).OR.(MSTJ(41).GE.2.AND.
     &MIN(ZC,ZCE).GT.0.4999D0)).AND.IPSPD.EQ.0) THEN
        P(IEP(1),5)=PMTH(1,IR)
        V(IEP(1),5)=P(IEP(1),5)**2
        GOTO 450
      ENDIF
 
C...Integral of Altarelli-Parisi z kernel for QCD.
C...(Includes squark and gluino; with factor N_C/C_F extra for latter).
      IF(MSTJ(49).EQ.0.AND.KFL(1).EQ.21) THEN
Cacs+
C      FBR= 6D0*LOG((1D0-ZC)/ZC)+MSTJ(45)*0.5D0
      FBR=dgauss1(splitg1,zc,1.d0-zc,1.d-3)
Cacs-
C...QUARKONIA+++
C...Evolution of QQ~[3S18] state if MSTP(148)=1.
      ELSEIF(MSTJ(49).EQ.0.AND.MSTP(149).GE.0.AND.
     &       (KFL(1).EQ.9900443.OR.KFL(1).EQ.9900553)) THEN
        FBR=6D0*LOG((1D0-ZC)/ZC)
C...QUARKONIA---
      ELSEIF(MSTJ(49).EQ.0) THEN
Cacs+
C      FBR=(8D0/3D0)*LOG((1D0-ZC)/ZC)
      FBR=dgauss1(splitq1,zc,1.d0-zc,1.d-3) 
Cacs-
        IF(IGLUI.EQ.1.AND.IR.GE.31) FBR=FBR*(9D0/4D0)
 
C...Integral of Altarelli-Parisi z kernel for scalar gluon.
      ELSEIF(MSTJ(49).EQ.1.AND.KFL(1).EQ.21) THEN
        FBR=(PARJ(87)+MSTJ(45)*PARJ(88))*(1D0-2D0*ZC)
      ELSEIF(MSTJ(49).EQ.1) THEN
        FBR=(1D0-2D0*ZC)/3D0
        IF(IGM.EQ.0.AND.M3JC.GE.1) FBR=4D0*FBR
 
C...Integral of Altarelli-Parisi z kernel for Abelian vector gluon.
      ELSEIF(KFL(1).EQ.21) THEN
        FBR=6D0*MSTJ(45)*(0.5D0-ZC)
      ELSE
        FBR=2D0*LOG((1D0-ZC)/ZC)
      ENDIF
 
C...Reset QCD probability for colourless.
      IF(ISCOL(IR).EQ.0) FBR=0D0
 
C...Integral of Altarelli-Parisi kernel for photon emission.
      FBRE=0D0
      IF(MSTJ(41).GE.2.AND.ISCHG(IR).EQ.1) THEN
        IF(KFL(1).LE.18) THEN
          FBRE=(KCHG(KFL(1),1)/3D0)**2*2D0*LOG((1D0-ZCE)/ZCE)
        ENDIF
        IF(MSTJ(41).EQ.10) FBRE=PARJ(84)*FBRE
      ENDIF
 
C...Inner veto algorithm starts. Find maximum mass for evolution.
  410 PMS=V(IEP(1),5)
      IF(IGM.GE.0) THEN
        PM2=0D0
        DO 420 I=2,NEP
          PM=P(IEP(I),5)
          IRI=IREF(IEP(I)-NS)
          IF(KSH(IRI).EQ.1) PM=PMTH(2,IRI)
          PM2=PM2+PM
  420   CONTINUE
        PMS=MIN(PMS,(P(IM,5)-PM2)**2)
      ENDIF
 
C...Select mass for daughter in QCD evolution.
      B0=27D0/6D0
      DO 430 IFF=4,MSTJ(45)
        IF(PMS.GT.4D0*PMTH(2,IFF)**2) B0=(33D0-2D0*IFF)/6D0
  430 CONTINUE
C...Shift m^2 for evolution in Q^2 = m^2 - m(onshell)^2.
      PMSC=MAX(0.5D0*PARJ(82),PMS-PMTH(1,IR)**2)
C...Already predetermined choice.
      IF(IPSPD.NE.0) THEN
        PMSQCD=P(IPSPD,5)**2
      ELSEIF(FBR.LT.1D-3) THEN
        PMSQCD=0D0
      ELSEIF(MSTJ(44).LE.0) THEN
        PMSQCD=PMSC*EXP(MAX(-50D0,LOG(PYR(0))*PARU(2)/(PARU(111)*FBR)))
      ELSEIF(MSTJ(44).EQ.1) THEN
        PMSQCD=4D0*ALAMS*(0.25D0*PMSC/ALAMS)**(PYR(0)**(B0/FBR))
      ELSE
        PMSQCD=PMSC*EXP(MAX(-50D0,ALFM*B0*LOG(PYR(0))/FBR))
      ENDIF
C...Shift back m^2 from evolution in Q^2 = m^2 - m(onshell)^2.
      IF(IPSPD.EQ.0) PMSQCD=PMSQCD+PMTH(1,IR)**2
      IF(ZC.GT.0.49D0.OR.PMSQCD.LE.PMTH(4,IR)**2) PMSQCD=PMTH(2,IR)**2
      V(IEP(1),5)=PMSQCD
      MCE=1
 
C...Select mass for daughter in QED evolution.
      IF(MSTJ(41).GE.2.AND.ISCHG(IR).EQ.1.AND.IPSPD.EQ.0) THEN
C...Shift m^2 for evolution in Q^2 = m^2 - m(onshell)^2.
        PMSE=MAX(0.5D0*PARJ(83),PMS-PMTH(1,IR)**2)
        IF(FBRE.LT.1D-3) THEN
          PMSQED=0D0
        ELSE
          PMSQED=PMSE*EXP(MAX(-50D0,LOG(PYR(0))*PARU(2)/
     &    (PARU(101)*FBRE)))
        ENDIF
C...Shift back m^2 from evolution in Q^2 = m^2 - m(onshell)^2.
        PMSQED=PMSQED+PMTH(1,IR)**2
        IF(ZCE.GT.0.4999D0.OR.PMSQED.LE.PMTH(5,IR)**2) PMSQED=
     &  PMTH(2,IR)**2
        IF(PMSQED.GT.PMSQCD) THEN
          V(IEP(1),5)=PMSQED
          MCE=2
        ENDIF
      ENDIF

C...Check whether daughter mass below cutoff.
      P(IEP(1),5)=SQRT(V(IEP(1),5))
      IF(P(IEP(1),5).LE.PMTH(3,IR)) THEN
        P(IEP(1),5)=PMTH(1,IR)
        V(IEP(1),5)=P(IEP(1),5)**2
        GOTO 450
      ENDIF
Cacs+
       virt=V(IEP(1),5)
Cacs-
     
C...Already predetermined choice of z, and flavour in g -> qqbar.
      IF(IPSPD.NE.0) THEN
        IPSGD1=K(IPSPD,4)
        IPSGD2=K(IPSPD,5)
        PMSGD1=P(IPSGD1,5)**2
        PMSGD2=P(IPSGD2,5)**2
        ALAMPS=SQRT(MAX(1D-10,(PMSQCD-PMSGD1-PMSGD2)**2-
     &  4D0*PMSGD1*PMSGD2))
        Z=0.5D0*(PMSQCD*(2D0*P(IPSGD1,4)/P(IPSPD,4)-1D0)+ALAMPS-
     &  PMSGD1+PMSGD2)/ALAMPS
        Z=MAX(0.00001D0,MIN(0.99999D0,Z))
        IF(KFL(1).NE.21) THEN
          K(IEP(1),5)=21
        ELSE
          K(IEP(1),5)=IABS(K(IPSGD1,2))
        ENDIF
 
C...Select z value of branching: q -> qgamma.
      ELSEIF(MCE.EQ.2) THEN
        Z=1D0-(1D0-ZCE)*(ZCE/(1D0-ZCE))**PYR(0)
        IF(1D0+Z**2.LT.2D0*PYR(0)) GOTO 410
        K(IEP(1),5)=22

C...QUARKONIA+++
C...Select z value of branching: QQ~[3S18] -> QQ~[3S18]g.
      ELSEIF(MSTJ(49).EQ.0.AND.
     &       (KFL(1).EQ.9900443.OR.KFL(1).EQ.9900553)) THEN
        Z=(1D0-ZC)*(ZC/(1D0-ZC))**PYR(0)
C...Select always the harder 'gluon' if the switch MSTP(149)<=0.
        IF(MSTP(149).LE.0.OR.PYR(0).GT.0.5D0) Z=1D0-Z
        IF((1D0-Z*(1D0-Z))**2.LT.PYR(0)) GOTO 410
        K(IEP(1),5)=21
C...QUARKONIA---
 
C...Select z value of branching: q -> qg, g -> gg, g -> qqbar.
      ELSEIF(MSTJ(49).NE.1.AND.KFL(1).NE.21) THEN
Cacs+
C        Z=1D0-(1D0-ZC)*(ZC/(1D0-ZC))**PYR(0)
C...Only do z weighting when no ME correction afterwards.
C        IF(M3JC.EQ.0.AND.1D0+Z**2.LT.2D0*PYR(0)) GOTO 410
C 
        anfbr=dgauss1(splitq2,zc,1.d0-zc,1.d-3)
        z=simdis(500,zc,1,anfbr)
Cacs-
        K(IEP(1),5)=21 
      ELSEIF(MSTJ(49).EQ.0.AND.MSTJ(45)*0.5D0.LT.PYR(0)*FBR) THEN
Cacs+
c        Z=(1D0-ZC)*(ZC/(1D0-ZC))**PYR(0)
        anfbr=dgauss1(splitg2,zc,1.d0-zc,1.d-3) 
        z=simdis(500,zc,21,anfbr)
      
        IF(PYR(0).GT.0.5D0) Z=1D0-Z
c        IF((1D0-Z*(1D0-Z))**2.LT.PYR(0)) GOTO 410
Cacs-
        K(IEP(1),5)=21
      ELSEIF(MSTJ(49).NE.1) THEN
Cacs+
c        Z=PYR(0)
c        IF(Z**2+(1D0-Z)**2.LT.PYR(0)) GOTO 410
        anfbr=dgauss1(splitqqbar,zc,1.d0-zc,1.d-3)
        z=simdis(500,zc,3,anfbr)
Cacs-
        KFLB=1+INT(MSTJ(45)*PYR(0))
        PMQ=4D0*PMTH(2,KFLB)**2/V(IEP(1),5)
        IF(PMQ.GE.1D0) GOTO 410
        IF(MSTJ(44).LE.2.OR.MSTJ(44).EQ.4) THEN
          IF(Z.LT.ZC.OR.Z.GT.1D0-ZC) GOTO 410
          PMQ0=4D0*PMTH(2,21)**2/V(IEP(1),5)
          IF(MOD(MSTJ(43),2).EQ.0.AND.(1D0+0.5D0*PMQ)*SQRT(1D0-PMQ)
     &    .LT.PYR(0)*(1D0+0.5D0*PMQ0)*SQRT(1D0-PMQ0)) GOTO 410
        ELSE
          IF((1D0+0.5D0*PMQ)*SQRT(1D0-PMQ).LT.PYR(0)) GOTO 410
        ENDIF
        K(IEP(1),5)=KFLB
 
C...Ditto for scalar gluon model.
      ELSEIF(KFL(1).NE.21) THEN
        Z=1D0-SQRT(ZC**2+PYR(0)*(1D0-2D0*ZC))
        K(IEP(1),5)=21
      ELSEIF(PYR(0)*(PARJ(87)+MSTJ(45)*PARJ(88)).LE.PARJ(87)) THEN
        Z=ZC+(1D0-2D0*ZC)*PYR(0)
        K(IEP(1),5)=21
      ELSE
        Z=ZC+(1D0-2D0*ZC)*PYR(0)
        KFLB=1+INT(MSTJ(45)*PYR(0))
        PMQ=4D0*PMTH(2,KFLB)**2/V(IEP(1),5)
        IF(PMQ.GE.1D0) GOTO 410
        K(IEP(1),5)=KFLB
      ENDIF
 
C...Correct to alpha_s(pT^2) (optionally m^2/4 for g -> q qbar).
      IF(MCE.EQ.1.AND.MSTJ(44).GE.2.AND.IPSPD.EQ.0) THEN
        IF(KFL(1).EQ.21.AND.K(IEP(1),5).LT.10.AND.
     &  (MSTJ(44).EQ.3.OR.MSTJ(44).EQ.5)) THEN
          IF(ALFM/LOG(V(IEP(1),5)*0.25D0/ALAMS).LT.PYR(0)) GOTO 410
        ELSE
          PT2APP=Z*(1D0-Z)*V(IEP(1),5)
          IF(MSTJ(44).GE.4) PT2APP=PT2APP*
     &    (1D0-PMTH(1,IR)**2/V(IEP(1),5))**2
          IF(PT2APP.LT.PT2MIN) GOTO 410
          IF(ALFM/LOG(PT2APP/ALAMS).LT.PYR(0)) GOTO 410
        ENDIF
      ENDIF
 
C...Check if z consistent with chosen m.
      IF(KFL(1).EQ.21) THEN
        IRGD1=IABS(K(IEP(1),5))
        IRGD2=IRGD1
      ELSE
        IRGD1=IR
        IRGD2=IABS(K(IEP(1),5))
      ENDIF
      IF(NEP.EQ.1) THEN
        PED=PS(4)
      ELSEIF(NEP.GE.3) THEN
        PED=P(IEP(1),4)
      ELSEIF(IGM.EQ.0.OR.MSTJ(43).LE.2) THEN
        PED=0.5D0*(V(IM,5)+V(IEP(1),5)-PM2**2)/P(IM,5)
      ELSE
        IF(IEP(1).EQ.N+1) PED=V(IM,1)*PEM
        IF(IEP(1).EQ.N+2) PED=(1D0-V(IM,1))*PEM
      ENDIF
      IF(MOD(MSTJ(43),2).EQ.1) THEN
        PMQTH3=0.5D0*PARJ(82)
        IF(IRGD2.EQ.22) PMQTH3=0.5D0*PARJ(83)
        IF(IRGD2.EQ.22.AND.ISCOL(IR).EQ.0) PMQTH3=0.5D0*PARJ(90)
        PMQ1=(PMTH(1,IRGD1)**2+PMQTH3**2)/V(IEP(1),5)
        PMQ2=(PMTH(1,IRGD2)**2+PMQTH3**2)/V(IEP(1),5)
        ZD=SQRT(MAX(0D0,(1D0-V(IEP(1),5)/PED**2)*((1D0-PMQ1-PMQ2)**2-
     &  4D0*PMQ1*PMQ2)))
        ZH=1D0+PMQ1-PMQ2
      ELSE
        ZD=SQRT(MAX(0D0,1D0-V(IEP(1),5)/PED**2))
        ZH=1D0
      ENDIF
      IF(KFL(1).EQ.21.AND.K(IEP(1),5).LT.10.AND.
     &(MSTJ(44).EQ.3.OR.MSTJ(44).EQ.5)) THEN
      ELSEIF(IPSPD.NE.0) THEN
      ELSE
        ZL=0.5D0*(ZH-ZD)
        ZU=0.5D0*(ZH+ZD)
        IF(Z.LT.ZL.OR.Z.GT.ZU) GOTO 410
      ENDIF
      IF(KFL(1).EQ.21) V(IEP(1),3)=LOG(ZU*(1D0-ZL)/MAX(1D-20,ZL*
     &(1D0-ZU)))
      IF(KFL(1).NE.21) V(IEP(1),3)=LOG((1D0-ZL)/MAX(1D-10,1D0-ZU))
 
C...Width suppression for q -> q + g.
      IF(MSTJ(40).NE.0.AND.KFL(1).NE.21.AND.IPSPD.EQ.0) THEN
        IF(IGM.EQ.0) THEN
          EGLU=0.5D0*PS(5)*(1D0-Z)*(1D0+V(IEP(1),5)/V(NS+1,5))
        ELSE
          EGLU=PMED*(1D0-Z)
        ENDIF
        CHI=PARJ(89)**2/(PARJ(89)**2+EGLU**2)
        IF(MSTJ(40).EQ.1) THEN
          IF(CHI.LT.PYR(0)) GOTO 410
        ELSEIF(MSTJ(40).EQ.2) THEN
          IF(1D0-CHI.LT.PYR(0)) GOTO 410
        ENDIF
      ENDIF
 
C...Three-jet matrix element correction.
      IF(M3JC.GE.1) THEN
        WME=1D0
        WSHOW=1D0
 
C...QED matrix elements: only for massless case so far.
        IF(MCE.EQ.2.AND.IGM.EQ.0) THEN
          X1=Z*(1D0+V(IEP(1),5)/V(NS+1,5))
          X2=1D0-V(IEP(1),5)/V(NS+1,5)
          X3=(1D0-X1)+(1D0-X2)
          KI1=K(IPA(INUM),2)
          KI2=K(IPA(3-INUM),2)
          QF1=KCHG(PYCOMP(KI1),1)*ISIGN(1,KI1)/3D0
          QF2=KCHG(PYCOMP(KI2),1)*ISIGN(1,KI2)/3D0
          WSHOW=QF1**2*(1D0-X1)/X3*(1D0+(X1/(2D0-X2))**2)+
     &    QF2**2*(1D0-X2)/X3*(1D0+(X2/(2D0-X1))**2)
          WME=(QF1*(1D0-X1)/X3-QF2*(1D0-X2)/X3)**2*(X1**2+X2**2)
        ELSEIF(MCE.EQ.2) THEN
 
C...QCD matrix elements, including mass effects.
        ELSEIF(MSTJ(49).NE.1.AND.K(IEP(1),2).NE.21) THEN
          PS1ME=V(IEP(1),5)
          PM1ME=PMTH(1,IR)
          M3JCC=M3JC
          IF(IR.GE.31.AND.IGM.EQ.0) THEN
C...QCD ME: original parton, first branching.
            PM2ME=PMTH(1,63-IR)
            ECMME=PS(5)
          ELSEIF(IR.GE.31) THEN
C...QCD ME: original parton, subsequent branchings.
            PM2ME=PMTH(1,63-IR)
            PEDME=PEM*(V(IM,1)+(1D0-V(IM,1))*PS1ME/V(IM,5))
            ECMME=PEDME+SQRT(MAX(0D0,PEDME**2-PS1ME+PM2ME**2))
          ELSEIF(K(IM,2).EQ.21) THEN
C...QCD ME: secondary partons, first branching.
            PM2ME=PM1ME
            ZMME=V(IM,1)
            IF(IEP(1).GT.IEP(2)) ZMME=1D0-ZMME
            PMLME=SQRT(MAX(0D0,(V(IM,5)-PS1ME-PM2ME**2)**2-
     &      4D0*PS1ME*PM2ME**2))
            PEDME=PEM*(0.5D0*(V(IM,5)-PMLME+PS1ME-PM2ME**2)+PMLME*ZMME)/
     &      V(IM,5)
            ECMME=PEDME+SQRT(MAX(0D0,PEDME**2-PS1ME+PM2ME**2))
            M3JCC=66
          ELSE
C...QCD ME: secondary partons, subsequent branchings.
            PM2ME=PM1ME
            PEDME=PEM*(V(IM,1)+(1D0-V(IM,1))*PS1ME/V(IM,5))
            ECMME=PEDME+SQRT(MAX(0D0,PEDME**2-PS1ME+PM2ME**2))
            M3JCC=66
          ENDIF
C...Construct ME variables.
          R1ME=PM1ME/ECMME
          R2ME=PM2ME/ECMME
          X1=(1D0+PS1ME/ECMME**2-R2ME**2)*(Z+(1D0-Z)*PM1ME**2/PS1ME)
          X2=1D0+R2ME**2-PS1ME/ECMME**2
C...Call ME, with right order important for two inequivalent showerers.
          IF(IR.EQ.IORD+30) THEN
            WME=PYMAEL(M3JCC,X1,X2,R1ME,R2ME,ALPHA)
          ELSE
            WME=PYMAEL(M3JCC,X2,X1,R2ME,R1ME,ALPHA)
          ENDIF
C...Split up total ME when two radiating partons.
          ISPRAD=1
          IF((M3JCC.GE.16.AND.M3JCC.LE.19).OR.
     &    (M3JCC.GE.26.AND.M3JCC.LE.29).OR.
     &    (M3JCC.GE.36.AND.M3JCC.LE.39).OR.
     &    (M3JCC.GE.46.AND.M3JCC.LE.49).OR.
     &    (M3JCC.GE.56.AND.M3JCC.LE.64)) ISPRAD=0
          IF(ISPRAD.EQ.1) WME=WME*MAX(1D-10,1D0+R1ME**2-R2ME**2-X1)/
     &    MAX(1D-10,2D0-X1-X2)
C...Evaluate shower rate to be compared with.
          WSHOW=2D0/(MAX(1D-10,2D0-X1-X2)*
     &    MAX(1D-10,1D0+R2ME**2-R1ME**2-X2))
          IF(IGLUI.EQ.1.AND.IR.GE.31) WSHOW=(9D0/4D0)*WSHOW
        ELSEIF(MSTJ(49).NE.1) THEN
 
C...Toy model scalar theory matrix elements; no mass effects.
        ELSE
          X1=Z*(1D0+V(IEP(1),5)/V(NS+1,5))
          X2=1D0-V(IEP(1),5)/V(NS+1,5)
          X3=(1D0-X1)+(1D0-X2)
          WSHOW=4D0*X3*((1D0-X1)/(2D0-X2)**2+(1D0-X2)/(2D0-X1)**2)
          WME=X3**2
          IF(MSTJ(102).GE.2) WME=X3**2-2D0*(1D0+X3)*(1D0-X1)*(1D0-X2)*
     &    PARJ(171)
        ENDIF
 
        IF(WME.LT.PYR(0)*WSHOW) GOTO 410
      ENDIF
 
C...Impose angular ordering by rejection of nonordered emission.
      IF(MCE.EQ.1.AND.IGM.GT.0.AND.MSTJ(42).GE.2.AND.IPSPD.EQ.0) THEN
        PEMAO=V(IM,1)*P(IM,4)
        IF(IEP(1).EQ.N+2) PEMAO=(1D0-V(IM,1))*P(IM,4)
        IF(IR.GE.31.AND.MSTJ(42).GE.5) THEN
          MAOD=0
        ELSEIF(KFL(1).EQ.21.AND.K(IEP(1),5).LE.10.AND.(MSTJ(42).EQ.4
     &  .OR.MSTJ(42).EQ.7)) THEN
          MAOD=0
        ELSEIF(KFL(1).EQ.21.AND.K(IEP(1),5).LE.10.AND.(MSTJ(42).EQ.3
     &  .OR.MSTJ(42).EQ.6)) THEN
          MAOD=1
          PMDAO=PMTH(2,K(IEP(1),5))
          THE2ID=Z*(1D0-Z)*PEMAO**2/(V(IEP(1),5)-4D0*PMDAO**2)
        ELSE
          MAOD=1
          THE2ID=Z*(1D0-Z)*PEMAO**2/V(IEP(1),5)
          IF(MSTJ(42).GE.3.AND.MSTJ(42).NE.5) THE2ID=THE2ID*
     &    (1D0+PMTH(1,IR)**2*(1D0-Z)/(V(IEP(1),5)*Z))**2
        ENDIF
        MAOM=1
        IAOM=IM
  440   IF(K(IAOM,5).EQ.22) THEN
          IAOM=K(IAOM,3)
          IF(K(IAOM,3).LE.NS) MAOM=0
          IF(MAOM.EQ.1) GOTO 440
        ENDIF
        IF(MAOM.EQ.1.AND.MAOD.EQ.1) THEN
          THE2IM=V(IAOM,1)*(1D0-V(IAOM,1))*P(IAOM,4)**2/V(IAOM,5)
          IF(THE2ID.LT.THE2IM) GOTO 410
        ENDIF
      ENDIF
 
C...Impose user-defined maximum angle at first branching.
      IF(MSTJ(48).EQ.1.AND.IPSPD.EQ.0) THEN
        IF(NEP.EQ.1.AND.IM.EQ.NS) THEN
          THE2ID=Z*(1D0-Z)*PS(4)**2/V(IEP(1),5)
          IF(PARJ(85)**2*THE2ID.LT.1D0) GOTO 410
        ELSEIF(NEP.EQ.2.AND.IEP(1).EQ.NS+2) THEN
          THE2ID=Z*(1D0-Z)*(0.5D0*P(IM,4))**2/V(IEP(1),5)
          IF(PARJ(85)**2*THE2ID.LT.1D0) GOTO 410
        ELSEIF(NEP.EQ.2.AND.IEP(1).EQ.NS+3) THEN
          THE2ID=Z*(1D0-Z)*(0.5D0*P(IM,4))**2/V(IEP(1),5)
          IF(PARJ(86)**2*THE2ID.LT.1D0) GOTO 410
        ENDIF
      ENDIF
 
C...Impose angular constraint in first branching from interference
C...with initial state partons.
      IF(MIIS.GE.2.AND.IEP(1).LE.NS+3) THEN
        THE2D=MAX((1D0-Z)/Z,Z/(1D0-Z))*V(IEP(1),5)/(0.5D0*P(IM,4))**2
        IF(IEP(1).EQ.NS+2.AND.ISII(1).GE.1) THEN
          IF(THE2D.GT.THEIIS(1,ISII(1))**2) GOTO 410
        ELSEIF(IEP(1).EQ.NS+3.AND.ISII(2).GE.1) THEN
          IF(THE2D.GT.THEIIS(2,ISII(2))**2) GOTO 410
        ENDIF
      ENDIF
 
C...End of inner veto algorithm. Check if only one leg evolved so far.
  450 V(IEP(1),1)=Z
      ISL(1)=0
      ISL(2)=0
      IF(NEP.EQ.1) GOTO 490
      IF(NEP.EQ.2.AND.P(IEP(1),5)+P(IEP(2),5).GE.P(IM,5)) GOTO 350
      DO 460 I=1,NEP
        IR=IREF(N+I-NS)
        IF(ITRY(I).EQ.0.AND.KSH(IR).EQ.1) THEN
          IF(P(N+I,5).GE.PMTH(2,IR)) GOTO 350
        ENDIF
  460 CONTINUE
 
C...Check if chosen multiplet m1,m2,z1,z2 is physical.
      IF(NEP.GE.3) THEN
        PMSUM=0D0
        DO 470 I=1,NEP
          PMSUM=PMSUM+P(N+I,5)
  470   CONTINUE
        IF(PMSUM.GE.PS(5)) GOTO 350
      ELSEIF(IGM.EQ.0.OR.MSTJ(43).LE.2.OR.MOD(MSTJ(43),2).EQ.0) THEN
        DO 480 I1=N+1,N+2
          IRDA=IREF(I1-NS)
          IF(KSH(IRDA).EQ.0) GOTO 480
          IF(P(I1,5).LT.PMTH(2,IRDA)) GOTO 480
          IF(IRDA.EQ.21) THEN
            IRGD1=IABS(K(I1,5))
            IRGD2=IRGD1
          ELSE
            IRGD1=IRDA
            IRGD2=IABS(K(I1,5))
          ENDIF
          I2=2*N+3-I1
          IF(IGM.EQ.0.OR.MSTJ(43).LE.2) THEN
            PED=0.5D0*(V(IM,5)+V(I1,5)-V(I2,5))/P(IM,5)
          ELSE
            IF(I1.EQ.N+1) ZM=V(IM,1)
            IF(I1.EQ.N+2) ZM=1D0-V(IM,1)
            PML=SQRT((V(IM,5)-V(N+1,5)-V(N+2,5))**2-
     &      4D0*V(N+1,5)*V(N+2,5))
            PED=PEM*(0.5D0*(V(IM,5)-PML+V(I1,5)-V(I2,5))+PML*ZM)/
     &      V(IM,5)
          ENDIF
          IF(MOD(MSTJ(43),2).EQ.1) THEN
            PMQTH3=0.5D0*PARJ(82)
            IF(IRGD2.EQ.22) PMQTH3=0.5D0*PARJ(83)
            IF(IRGD2.EQ.22.AND.ISCOL(IRDA).EQ.0) PMQTH3=0.5D0*PARJ(90)
            PMQ1=(PMTH(1,IRGD1)**2+PMQTH3**2)/V(I1,5)
            PMQ2=(PMTH(1,IRGD2)**2+PMQTH3**2)/V(I1,5)
            ZD=SQRT(MAX(0D0,(1D0-V(I1,5)/PED**2)*((1D0-PMQ1-PMQ2)**2-
     &      4D0*PMQ1*PMQ2)))
            ZH=1D0+PMQ1-PMQ2
          ELSE
            ZD=SQRT(MAX(0D0,1D0-V(I1,5)/PED**2))
            ZH=1D0
          ENDIF
          IF(IRDA.EQ.21.AND.IRGD1.LT.10.AND.
     &    (MSTJ(44).EQ.3.OR.MSTJ(44).EQ.5)) THEN
          ELSE
            ZL=0.5D0*(ZH-ZD)
            ZU=0.5D0*(ZH+ZD)
            IF(I1.EQ.N+1.AND.(V(I1,1).LT.ZL.OR.V(I1,1).GT.ZU).AND.
     &      ISSET(1).EQ.0) THEN
              ISL(1)=1
            ELSEIF(I1.EQ.N+2.AND.(V(I1,1).LT.ZL.OR.V(I1,1).GT.ZU).AND.
     &      ISSET(2).EQ.0) THEN
              ISL(2)=1
            ENDIF
          ENDIF
          IF(IRDA.EQ.21) V(I1,4)=LOG(ZU*(1D0-ZL)/MAX(1D-20,
     &    ZL*(1D0-ZU)))
          IF(IRDA.NE.21) V(I1,4)=LOG((1D0-ZL)/MAX(1D-10,1D0-ZU))
  480   CONTINUE
        IF(ISL(1).EQ.1.AND.ISL(2).EQ.1.AND.ISLM.NE.0) THEN
          ISL(3-ISLM)=0
          ISLM=3-ISLM
        ELSEIF(ISL(1).EQ.1.AND.ISL(2).EQ.1) THEN
          ZDR1=MAX(0D0,V(N+1,3)/MAX(1D-6,V(N+1,4))-1D0)
          ZDR2=MAX(0D0,V(N+2,3)/MAX(1D-6,V(N+2,4))-1D0)
          IF(ZDR2.GT.PYR(0)*(ZDR1+ZDR2)) ISL(1)=0
          IF(ISL(1).EQ.1) ISL(2)=0
          IF(ISL(1).EQ.0) ISLM=1
          IF(ISL(2).EQ.0) ISLM=2
        ENDIF
        IF(ISL(1).EQ.1.OR.ISL(2).EQ.1) GOTO 350
      ENDIF
      IRD1=IREF(N+1-NS)
      IRD2=IREF(N+2-NS)
      IF(IGM.GT.0) THEN
        IF(MOD(MSTJ(43),2).EQ.1.AND.(P(N+1,5).GE.
     &  PMTH(2,IRD1).OR.P(N+2,5).GE.PMTH(2,IRD2))) THEN
          PMQ1=V(N+1,5)/V(IM,5)
          PMQ2=V(N+2,5)/V(IM,5)
          ZD=SQRT(MAX(0D0,(1D0-V(IM,5)/PEM**2)*((1D0-PMQ1-PMQ2)**2-
     &    4D0*PMQ1*PMQ2)))
          ZH=1D0+PMQ1-PMQ2
          ZL=0.5D0*(ZH-ZD)
          ZU=0.5D0*(ZH+ZD)
          IF(V(IM,1).LT.ZL.OR.V(IM,1).GT.ZU) GOTO 350
        ENDIF
      ENDIF
 
C...Accepted branch. Construct four-momentum for initial partons.
  490 MAZIP=0
      MAZIC=0
      IF(NEP.EQ.1) THEN
        P(N+1,1)=0D0
        P(N+1,2)=0D0
        P(N+1,3)=SQRT(MAX(0D0,(P(IPA(1),4)+P(N+1,5))*(P(IPA(1),4)-
     &  P(N+1,5))))
        P(N+1,4)=P(IPA(1),4)
        V(N+1,2)=P(N+1,4)
      ELSEIF(IGM.EQ.0.AND.NEP.EQ.2) THEN
        PED1=0.5D0*(V(IM,5)+V(N+1,5)-V(N+2,5))/P(IM,5)
        P(N+1,1)=0D0
        P(N+1,2)=0D0
        P(N+1,3)=SQRT(MAX(0D0,(PED1+P(N+1,5))*(PED1-P(N+1,5))))
        P(N+1,4)=PED1
        P(N+2,1)=0D0
        P(N+2,2)=0D0
        P(N+2,3)=-P(N+1,3)
        P(N+2,4)=P(IM,5)-PED1
        V(N+1,2)=P(N+1,4)
        V(N+2,2)=P(N+2,4)
      ELSEIF(NEP.GE.3) THEN
C...Rescale all momenta for energy conservation.
        LOOP=0
        PES=0D0
        PQS=0D0
        DO 510 I=1,NEP
          DO 500 J=1,4
            P(N+I,J)=P(IPA(I),J)
  500     CONTINUE
          PES=PES+P(N+I,4)
          PQS=PQS+P(N+I,5)**2/P(N+I,4)
  510   CONTINUE
  520   LOOP=LOOP+1
        FAC=(PS(5)-PQS)/(PES-PQS)
        PES=0D0
        PQS=0D0
        DO 540 I=1,NEP
          DO 530 J=1,3
            P(N+I,J)=FAC*P(N+I,J)
  530     CONTINUE
          P(N+I,4)=SQRT(P(N+I,5)**2+P(N+I,1)**2+P(N+I,2)**2+P(N+I,3)**2)
          V(N+I,2)=P(N+I,4)
          PES=PES+P(N+I,4)
          PQS=PQS+P(N+I,5)**2/P(N+I,4)
  540   CONTINUE
        IF(LOOP.LT.10.AND.ABS(PES-PS(5)).GT.1D-12*PS(5)) GOTO 520
 
C...Construct transverse momentum for ordinary branching in shower.
      ELSE
        ZM=V(IM,1)
        LOOPPT=0
  550   LOOPPT=LOOPPT+1
        PZM=SQRT(MAX(0D0,(PEM+P(IM,5))*(PEM-P(IM,5))))
        PMLS=(V(IM,5)-V(N+1,5)-V(N+2,5))**2-4D0*V(N+1,5)*V(N+2,5)
        IF(PZM.LE.0D0) THEN
          PTS=0D0
        ELSEIF(K(IM,2).EQ.21.AND.IABS(K(N+1,2)).LE.10.AND.
     &  (MSTJ(44).EQ.3.OR.MSTJ(44).EQ.5)) THEN
          PTS=PMLS*ZM*(1D0-ZM)/V(IM,5)
        ELSEIF(MOD(MSTJ(43),2).EQ.1) THEN
          PTS=(PEM**2*(ZM*(1D0-ZM)*V(IM,5)-(1D0-ZM)*V(N+1,5)-
     &    ZM*V(N+2,5))-0.25D0*PMLS)/PZM**2
        ELSE
          PTS=PMLS*(ZM*(1D0-ZM)*PEM**2/V(IM,5)-0.25D0)/PZM**2
        ENDIF
        IF(PTS.LT.0D0.AND.LOOPPT.LT.10) THEN
          ZM=0.05D0+0.9D0*ZM
          GOTO 550
        ELSEIF(PTS.LT.0D0) THEN
          GOTO 280
        ENDIF
        PT=SQRT(MAX(0D0,PTS))
 
C...Global statistics.
        MINT(353)=MINT(353)+1
        VINT(353)=VINT(353)+PT
        IF (MINT(353).EQ.1) VINT(358)=PT
 
C...Find coefficient of azimuthal asymmetry due to gluon polarization.
        HAZIP=0D0
        IF(MSTJ(49).NE.1.AND.MOD(MSTJ(46),2).EQ.1.AND.K(IM,2).EQ.21
     &  .AND.IAU.NE.0) THEN
          IF(K(IGM,3).NE.0) MAZIP=1
          ZAU=V(IGM,1)
          IF(IAU.EQ.IM+1) ZAU=1D0-V(IGM,1)
          IF(MAZIP.EQ.0) ZAU=0D0
          IF(K(IGM,2).NE.21) THEN
            HAZIP=2D0*ZAU/(1D0+ZAU**2)
          ELSE
            HAZIP=(ZAU/(1D0-ZAU*(1D0-ZAU)))**2
          ENDIF
          IF(K(N+1,2).NE.21) THEN
            HAZIP=HAZIP*(-2D0*ZM*(1D0-ZM))/(1D0-2D0*ZM*(1D0-ZM))
          ELSE
            HAZIP=HAZIP*(ZM*(1D0-ZM)/(1D0-ZM*(1D0-ZM)))**2
          ENDIF
        ENDIF
 
C...Find coefficient of azimuthal asymmetry due to soft gluon
C...interference.
        HAZIC=0D0
        IF(MSTJ(49).NE.2.AND.MSTJ(46).GE.2.AND.(K(N+1,2).EQ.21.OR.
     &  K(N+2,2).EQ.21).AND.IAU.NE.0) THEN
          IF(K(IGM,3).NE.0) MAZIC=N+1
          IF(K(IGM,3).NE.0.AND.K(N+1,2).NE.21) MAZIC=N+2
          IF(K(IGM,3).NE.0.AND.K(N+1,2).EQ.21.AND.K(N+2,2).EQ.21.AND.
     &    ZM.GT.0.5D0) MAZIC=N+2
          IF(K(IAU,2).EQ.22) MAZIC=0
          ZS=ZM
          IF(MAZIC.EQ.N+2) ZS=1D0-ZM
          ZGM=V(IGM,1)
          IF(IAU.EQ.IM-1) ZGM=1D0-V(IGM,1)
          IF(MAZIC.EQ.0) ZGM=1D0
          IF(MAZIC.NE.0) HAZIC=(P(IM,5)/P(IGM,5))*
     &    SQRT((1D0-ZS)*(1D0-ZGM)/(ZS*ZGM))
          HAZIC=MIN(0.95D0,HAZIC)
        ENDIF
      ENDIF
 
C...Construct energies for ordinary branching in shower.
  560 IF(NEP.EQ.2.AND.IGM.GT.0) THEN
        IF(K(IM,2).EQ.21.AND.IABS(K(N+1,2)).LE.10.AND.
     &  (MSTJ(44).EQ.3.OR.MSTJ(44).EQ.5)) THEN
          P(N+1,4)=0.5D0*(PEM*(V(IM,5)+V(N+1,5)-V(N+2,5))+
     &    PZM*SQRT(MAX(0D0,PMLS))*(2D0*ZM-1D0))/V(IM,5)
        ELSEIF(MOD(MSTJ(43),2).EQ.1) THEN
          P(N+1,4)=PEM*V(IM,1)
        ELSE
          P(N+1,4)=PEM*(0.5D0*(V(IM,5)-SQRT(PMLS)+V(N+1,5)-V(N+2,5))+
     &    SQRT(PMLS)*ZM)/V(IM,5)
        ENDIF
 
C...Already predetermined choice of phi angle or not
        PHI=PARU(2)*PYR(0)
        IF(MPSPD.EQ.1.AND.IGM.EQ.NS+1) THEN
          IPSPD=IP1+IM-NS-2
          IF(K(IPSPD,4).GT.0) THEN
            IPSGD1=K(IPSPD,4)
            IF(IM.EQ.NS+2) THEN
              PHI=PYANGL(P(IPSGD1,1),P(IPSGD1,2))
            ELSE
              PHI=PYANGL(-P(IPSGD1,1),P(IPSGD1,2))
            ENDIF
          ENDIF
        ELSEIF(MPSPD.EQ.1.AND.IGM.EQ.NS+2) THEN
          IPSPD=IP1+IM-NS-2
          IF(K(IPSPD,4).GT.0) THEN
            IPSGD1=K(IPSPD,4)
            PHIPSM=PYANGL(P(IPSPD,1),P(IPSPD,2))
            THEPSM=PYANGL(P(IPSPD,3),SQRT(P(IPSPD,1)**2+P(IPSPD,2)**2))
            CALL PYROBO(IPSGD1,IPSGD1,0D0,-PHIPSM,0D0,0D0,0D0)
            CALL PYROBO(IPSGD1,IPSGD1,-THEPSM,0D0,0D0,0D0,0D0)
            PHI=PYANGL(P(IPSGD1,1),P(IPSGD1,2))
            CALL PYROBO(IPSGD1,IPSGD1,THEPSM,PHIPSM,0D0,0D0,0D0)
          ENDIF
        ENDIF
 
C...Construct momenta for ordinary branching in shower.
        P(N+1,1)=PT*COS(PHI)
        P(N+1,2)=PT*SIN(PHI)
        IF(K(IM,2).EQ.21.AND.IABS(K(N+1,2)).LE.10.AND.
     &  (MSTJ(44).EQ.3.OR.MSTJ(44).EQ.5)) THEN
          P(N+1,3)=0.5D0*(PZM*(V(IM,5)+V(N+1,5)-V(N+2,5))+
     &    PEM*SQRT(MAX(0D0,PMLS))*(2D0*ZM-1D0))/V(IM,5)
        ELSEIF(PZM.GT.0D0) THEN
          P(N+1,3)=0.5D0*(V(N+2,5)-V(N+1,5)-V(IM,5)+
     &    2D0*PEM*P(N+1,4))/PZM
        ELSE
          P(N+1,3)=0D0
        ENDIF
        P(N+2,1)=-P(N+1,1)
        P(N+2,2)=-P(N+1,2)
        P(N+2,3)=PZM-P(N+1,3)
        P(N+2,4)=PEM-P(N+1,4)
        IF(MSTJ(43).LE.2) THEN
          V(N+1,2)=(PEM*P(N+1,4)-PZM*P(N+1,3))/P(IM,5)
          V(N+2,2)=(PEM*P(N+2,4)-PZM*P(N+2,3))/P(IM,5)
        ENDIF
      ENDIF
 
C...Rotate and boost daughters.
      IF(IGM.GT.0) THEN
        IF(MSTJ(43).LE.2) THEN
          BEX=P(IGM,1)/P(IGM,4)
          BEY=P(IGM,2)/P(IGM,4)
          BEZ=P(IGM,3)/P(IGM,4)
          GA=P(IGM,4)/P(IGM,5)
          GABEP=GA*(GA*(BEX*P(IM,1)+BEY*P(IM,2)+BEZ*P(IM,3))/(1D0+GA)-
     &    P(IM,4))
        ELSE
          BEX=0D0
          BEY=0D0
          BEZ=0D0
          GA=1D0
          GABEP=0D0
        ENDIF
        PTIMB=SQRT((P(IM,1)+GABEP*BEX)**2+(P(IM,2)+GABEP*BEY)**2)
        THE=PYANGL(P(IM,3)+GABEP*BEZ,PTIMB)
        IF(PTIMB.GT.1D-4) THEN
          PHI=PYANGL(P(IM,1)+GABEP*BEX,P(IM,2)+GABEP*BEY)
        ELSE
          PHI=0D0
        ENDIF
        DO 570 I=N+1,N+2
          DP(1)=COS(THE)*COS(PHI)*P(I,1)-SIN(PHI)*P(I,2)+
     &    SIN(THE)*COS(PHI)*P(I,3)
          DP(2)=COS(THE)*SIN(PHI)*P(I,1)+COS(PHI)*P(I,2)+
     &    SIN(THE)*SIN(PHI)*P(I,3)
          DP(3)=-SIN(THE)*P(I,1)+COS(THE)*P(I,3)
          DP(4)=P(I,4)
          DBP=BEX*DP(1)+BEY*DP(2)+BEZ*DP(3)
          DGABP=GA*(GA*DBP/(1D0+GA)+DP(4))
          P(I,1)=DP(1)+DGABP*BEX
          P(I,2)=DP(2)+DGABP*BEY
          P(I,3)=DP(3)+DGABP*BEZ
          P(I,4)=GA*(DP(4)+DBP)
  570   CONTINUE
      ENDIF
 
C...Weight with azimuthal distribution, if required.
      IF(MAZIP.NE.0.OR.MAZIC.NE.0) THEN
        DO 580 J=1,3
          DPT(1,J)=P(IM,J)
          DPT(2,J)=P(IAU,J)
          DPT(3,J)=P(N+1,J)
  580   CONTINUE
        DPMA=DPT(1,1)*DPT(2,1)+DPT(1,2)*DPT(2,2)+DPT(1,3)*DPT(2,3)
        DPMD=DPT(1,1)*DPT(3,1)+DPT(1,2)*DPT(3,2)+DPT(1,3)*DPT(3,3)
        DPMM=DPT(1,1)**2+DPT(1,2)**2+DPT(1,3)**2
        DO 590 J=1,3
          DPT(4,J)=DPT(2,J)-DPMA*DPT(1,J)/MAX(1D-10,DPMM)
          DPT(5,J)=DPT(3,J)-DPMD*DPT(1,J)/MAX(1D-10,DPMM)
  590   CONTINUE
        DPT(4,4)=SQRT(DPT(4,1)**2+DPT(4,2)**2+DPT(4,3)**2)
        DPT(5,4)=SQRT(DPT(5,1)**2+DPT(5,2)**2+DPT(5,3)**2)
        IF(MIN(DPT(4,4),DPT(5,4)).GT.0.1D0*PARJ(82)) THEN
          CAD=(DPT(4,1)*DPT(5,1)+DPT(4,2)*DPT(5,2)+
     &    DPT(4,3)*DPT(5,3))/(DPT(4,4)*DPT(5,4))
          IF(MAZIP.NE.0) THEN
            IF(1D0+HAZIP*(2D0*CAD**2-1D0).LT.PYR(0)*(1D0+ABS(HAZIP)))
     &      GOTO 560
          ENDIF
          IF(MAZIC.NE.0) THEN
            IF(MAZIC.EQ.N+2) CAD=-CAD
            IF((1D0-HAZIC)*(1D0-HAZIC*CAD)/(1D0+HAZIC**2-2D0*HAZIC*CAD)
     &      .LT.PYR(0)) GOTO 560
          ENDIF
        ENDIF
      ENDIF
 
C...Azimuthal anisotropy due to interference with initial state partons.
      IF(MOD(MIIS,2).EQ.1.AND.IGM.EQ.NS+1.AND.(K(N+1,2).EQ.21.OR.
     &K(N+2,2).EQ.21)) THEN
        III=IM-NS-1
        IF(ISII(III).GE.1) THEN
          IAZIID=N+1
          IF(K(N+1,2).NE.21) IAZIID=N+2
          IF(K(N+1,2).EQ.21.AND.K(N+2,2).EQ.21.AND.
     &    P(N+1,4).GT.P(N+2,4)) IAZIID=N+2
          THEIID=PYANGL(P(IAZIID,3),SQRT(P(IAZIID,1)**2+P(IAZIID,2)**2))
          IF(III.EQ.2) THEIID=PARU(1)-THEIID
          PHIIID=PYANGL(P(IAZIID,1),P(IAZIID,2))
          HAZII=MIN(0.95D0,THEIID/THEIIS(III,ISII(III)))
          CAD=COS(PHIIID-PHIIIS(III,ISII(III)))
          PHIREL=ABS(PHIIID-PHIIIS(III,ISII(III)))
          IF(PHIREL.GT.PARU(1)) PHIREL=PARU(2)-PHIREL
          IF((1D0-HAZII)*(1D0-HAZII*CAD)/(1D0+HAZII**2-2D0*HAZII*CAD)
     &    .LT.PYR(0)) GOTO 560
        ENDIF
      ENDIF
 
C...Continue loop over partons that may branch, until none left.
      IF(IGM.GE.0) K(IM,1)=14
      N=N+NEP
      NEP=2
      IF(N.GT.MSTU(4)-MSTU(32)-10) THEN
        CALL PYERRM(11,'(QPYSHOW:) no more memory left in PYJETS')
        IF(MSTU(21).GE.1) N=NS
        IF(MSTU(21).GE.1) RETURN
      ENDIF
      GOTO 290
 
C...Set information on imagined shower initiator.
  600 IF(NPA.GE.2) THEN
        K(NS+1,1)=11
        K(NS+1,2)=94
        K(NS+1,3)=IP1
        IF(IP2.GT.0.AND.IP2.LT.IP1) K(NS+1,3)=IP2
        K(NS+1,4)=NS+2
        K(NS+1,5)=NS+1+NPA
        IIM=1
      ELSE
        IIM=0
      ENDIF
 
C...Reconstruct string drawing information.
      DO 610 I=NS+1+IIM,N
        KQ=KCHG(PYCOMP(K(I,2)),2)
        IF(K(I,1).LE.10.AND.K(I,2).EQ.22) THEN
          K(I,1)=1
        ELSEIF(K(I,1).LE.10.AND.IABS(K(I,2)).GE.11.AND.
     &    IABS(K(I,2)).LE.18) THEN
          K(I,1)=1
        ELSEIF(K(I,1).LE.10) THEN
          K(I,4)=MSTU(5)*(K(I,4)/MSTU(5))
          K(I,5)=MSTU(5)*(K(I,5)/MSTU(5))
        ELSEIF(K(MOD(K(I,4),MSTU(5))+1,2).NE.22) THEN
          ID1=MOD(K(I,4),MSTU(5))
          IF(KQ.EQ.1.AND.K(I,2).GT.0) ID1=MOD(K(I,4),MSTU(5))+1
          IF(KQ.EQ.2.AND.(K(ID1,2).EQ.21.OR.K(ID1+1,2).EQ.21).AND.
     &    PYR(0).GT.0.5D0) ID1=MOD(K(I,4),MSTU(5))+1
          ID2=2*MOD(K(I,4),MSTU(5))+1-ID1
          K(I,4)=MSTU(5)*(K(I,4)/MSTU(5))+ID1
          K(I,5)=MSTU(5)*(K(I,5)/MSTU(5))+ID2
          K(ID1,4)=K(ID1,4)+MSTU(5)*I
          K(ID1,5)=K(ID1,5)+MSTU(5)*ID2
          K(ID2,4)=K(ID2,4)+MSTU(5)*ID1
          K(ID2,5)=K(ID2,5)+MSTU(5)*I
        ELSE
          ID1=MOD(K(I,4),MSTU(5))
          ID2=ID1+1
          K(I,4)=MSTU(5)*(K(I,4)/MSTU(5))+ID1
          K(I,5)=MSTU(5)*(K(I,5)/MSTU(5))+ID1
          IF(KQ.EQ.1.OR.K(ID1,1).GE.11) THEN
            K(ID1,4)=K(ID1,4)+MSTU(5)*I
            K(ID1,5)=K(ID1,5)+MSTU(5)*I
          ELSE
            K(ID1,4)=0
            K(ID1,5)=0
          ENDIF
          K(ID2,4)=0
          K(ID2,5)=0
        ENDIF
  610 CONTINUE
 
C...Transformation from CM frame.
      IF(NPA.EQ.1) THEN
        THE=PYANGL(P(IPA(1),3),SQRT(P(IPA(1),1)**2+P(IPA(1),2)**2))
        PHI=PYANGL(P(IPA(1),1),P(IPA(1),2))
        MSTU(33)=1
        CALL PYROBO(NS+1,N,THE,PHI,0D0,0D0,0D0)
      ELSEIF(NPA.EQ.2) THEN
        BEX=PS(1)/PS(4)
        BEY=PS(2)/PS(4)
        BEZ=PS(3)/PS(4)
        GA=PS(4)/PS(5)
        GABEP=GA*(GA*(BEX*P(IPA(1),1)+BEY*P(IPA(1),2)+BEZ*P(IPA(1),3))
     &  /(1D0+GA)-P(IPA(1),4))
        THE=PYANGL(P(IPA(1),3)+GABEP*BEZ,SQRT((P(IPA(1),1)
     &  +GABEP*BEX)**2+(P(IPA(1),2)+GABEP*BEY)**2))
        PHI=PYANGL(P(IPA(1),1)+GABEP*BEX,P(IPA(1),2)+GABEP*BEY)
        MSTU(33)=1
        CALL PYROBO(NS+1,N,THE,PHI,BEX,BEY,BEZ)
      ELSE
        CALL PYROBO(IPA(1),IPA(NPA),0D0,0D0,PS(1)/PS(4),PS(2)/PS(4),
     &  PS(3)/PS(4))
        MSTU(33)=1
        CALL PYROBO(NS+1,N,0D0,0D0,PS(1)/PS(4),PS(2)/PS(4),PS(3)/PS(4))
      ENDIF

C...Decay vertex of shower.
      DO 630 I=NS+1,N
        DO 620 J=1,5
          V(I,J)=V(IP1,J)
  620   CONTINUE
  630 CONTINUE
 
C...Delete trivial shower, else connect initiators.
      IF(N.LE.NS+NPA+IIM) THEN
        N=NS
      ELSE
        DO 640 IP=1,NPA
          K(IPA(IP),1)=14
          K(IPA(IP),4)=K(IPA(IP),4)+NS+IIM+IP
          K(IPA(IP),5)=K(IPA(IP),5)+NS+IIM+IP
          K(NS+IIM+IP,3)=IPA(IP)
          IF(IIM.EQ.1.AND.MSTU(16).NE.2) K(NS+IIM+IP,3)=NS+1
          IF(K(NS+IIM+IP,1).NE.1) THEN
            K(NS+IIM+IP,4)=MSTU(5)*IPA(IP)+K(NS+IIM+IP,4)
            K(NS+IIM+IP,5)=MSTU(5)*IPA(IP)+K(NS+IIM+IP,5)
          ENDIF
  640   CONTINUE
      ENDIF
 
      RETURN
      END
C
      SUBROUTINE QPYROBO(XI,YI,ZI,TI,THE,PHI,BEX,BEY,BEZ,XP,YP,ZP,TP)
C     N. Armesto, 16.04.2009
C     performs a boost and rotation of (t,x,y,z) to (tp,xp,yp,zp):
C     cut version of PYROBO, angles and boost parameters identical.
C
C     Note: to perform the transformation from the cms of the collision to
C     the cms of the hard scattering (as done at the beginning of the
C     shower in QPYSHOW or in qpygin), use the following sequence:
C     - qpyrobo with arguments 0.d0,0.d0,bbx,bby,bbz.
C     - qpyrobo with arguments 0.d0,aa2,0.d0,0.d0,0.d0.
C     - qpyrobo with arguments aa1,0.d0,0.d0,0.d0,0.d0.
C     For the inverse transformation (as done at the end of QPYSHOW or in
C     qpygeo), use a single call to qpyrobo with arguments
C     -aa1,-aa2,-bbx,-bby,-bbz.
C
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
C...Local arrays.
      DIMENSION ROT(3,3),VR(3),DV(4)
C
      X=XI
      Y=YI
      Z=ZI
      T=TI
C...Rotate, typically from z axis to direction (theta,phi).
      IF(THE**2+PHI**2.GT.1D-20) THEN
        ROT(1,1)=COS(THE)*COS(PHI)
        ROT(1,2)=-SIN(PHI)
        ROT(1,3)=SIN(THE)*COS(PHI)
        ROT(2,1)=COS(THE)*SIN(PHI)
        ROT(2,2)=COS(PHI)
        ROT(2,3)=SIN(THE)*SIN(PHI)
        ROT(3,1)=-SIN(THE)
        ROT(3,2)=0D0
        ROT(3,3)=COS(THE)
C   Instead of loop 120 in PYROBO.
        VR(1)=X
        VR(2)=Y
        VR(3)=Z
C   Instead of loop 130 in PYROBO.
        J=1
        X=ROT(J,1)*VR(1)+ROT(J,2)*VR(2)+ROT(J,3)*VR(3)
        J=2
        Y=ROT(J,1)*VR(1)+ROT(J,2)*VR(2)+ROT(J,3)*VR(3)
        J=3
        Z=ROT(J,1)*VR(1)+ROT(J,2)*VR(2)+ROT(J,3)*VR(3)
      ENDIF
C   If nothing happens...
      XP=X
      YP=Y
      ZP=Z
      TP=T
C...Boost, typically from rest to momentum/energy=beta.
      IF(BEX**2+BEY**2+BEZ**2.GT.1D-20) THEN
        DBX=BEX
        DBY=BEY
        DBZ=BEZ
        DB=SQRT(DBX**2+DBY**2+DBZ**2)
        EPS1=1D0-1D-12
        IF(DB.GT.EPS1) THEN
C...Rescale boost vector if too close to unity.
          CALL PYERRM(3,'(PYROBO:) boost vector too large')
          DBX=DBX*(EPS1/DB)
          DBY=DBY*(EPS1/DB)
          DBZ=DBZ*(EPS1/DB)
          DB=EPS1
        ENDIF
        DGA=1D0/SQRT(1D0-DB**2)
C    Instead of loop 150 in PYROBO.
        DV(1)=X
        DV(2)=Y
        DV(3)=Z
        DV(4)=T
        DBV=DBX*DV(1)+DBY*DV(2)+DBZ*DV(3)
        DGABV=DGA*(DGA*DBV/(1D0+DGA)+DV(4))
        XP=DV(1)+DGABV*DBX
        YP=DV(2)+DGABV*DBY
        ZP=DV(3)+DGABV*DBZ
        TP=DGA*(DV(4)+DBV)
      ENDIF
C 
      RETURN
      END
C
      SUBROUTINE QPYGIN(X0,Y0,Z0,T0)
C     NOT TO BE TOUCHED BY THE USER:
C     IT SETS THE INITIAL POSITION AND TIME OF THE
C     PARENT BRANCHING PARTON (X, Y, Z, T, IN FM) IN THE CENTER-OF-MASS
C     FRAME OF THE HARD COLLISION (IF APPLICABLE FOR THE TYPE OF EVENTS
C     YOU ARE SIMULATING). INFORMATION ABOUT THE BOOST AND ROTATION IS
C     CONTAINED IN THE IN COMMON QPLT BELOW.
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C     NOW THE COMMON CONTAINING THE VALUES OF THE TWO ANGLES AND THREE BOSST
C     PARAMETERS USED, IN QPYSHOW, TO CHANGE THROUGH PYROBO FROM THE
C     CENTER-OF-MASS OF THE COLLISION TO THE CENTER-OF-MASS OF THE HARD
C     SCATTERING. THEY ARE THE ENTRIES THREE TO SEVEN IN ROUTINE PYROBO.
C     XX, YY, ZZ, TT COMING FROM QPYGIN0 IN THE CMS OF THE COLLISION, GENERATED
C     ONCE PER NUCLEON-NUCLEON COLLISION.
      COMMON/TOQPYGIN/XX,YY,ZZ,TT
      COMMON/QPLT/AA1,AA2,BBX,BBY,BBZ
C     Boost and rotation.
      call qpyrobo(xx,yy,zz,tt,0.d0,0.d0,bbx,bby,bbz,x1,y1,z1,t1)
      call qpyrobo(x1,y1,z1,t1,0.d0,aa2,0.d0,0.d0,0.d0,x2,y2,z2,t2)
      call qpyrobo(x2,y2,z2,t2,aa1,0.d0,0.d0,0.d0,0.d0,x0,y0,z0,t0)
C
      RETURN
      END
C
      SUBROUTINE QPYGIN0
C     USER-DEFINED ROUTINE: IT SETS THE INITIAL POSITION AND TIME OF THE
C     PARENT BRANCHING PARTON (X, Y, Z, T, IN FM) IN THE CENTER-OF-MASS
C     FRAME OF THE COLLISION.
C     IT SHOULD BE CALLED IN THE MAIN PROGRAM, ONCE PER NUCLEON-NUCLEON
C     COLLISION (PER PP COLLISION IF YOU ARE SIMPLY RUNNING PP).
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER TAMOD
      COMMON/TOQPYGIN/XX,YY,ZZ,TT
      COMMON/NUCL/ANUCL,RA,BPAR,TAMOD
c... Position in the CM frame of the collision, by Konrad Tywoniuk.
      if(tamod.eq.1) then       ! jet prod from collision center
         xin=0.d0               ! fm
         yin=0.d0               ! fm
         zin=0.d0               ! fm
         tin=0.d0               ! fm
      else if(tamod.eq.2) then  ! jet prod distributed according to TAA(b)
         call qpyjetprod(xin,yin)
         zin=0.d0               ! fm
         tin=0.d0               ! fm
      else if(tamod.eq.3) then ! jet production along x-axis (for dijet study)
         xin=4.5d0
         yin=0.d0
         zin=0.d0
         tin=0.d0
      else if(tamod.eq.4) then ! jet production along y-axis (for dijet study)
         xin=0.d0
         yin=4.5d0
         zin=0.d0
         tin=0.d0
      else
         PRINT*,'Unkown geometry option: tamod=1,2...'
c         STOP
      end if
C     Passing to the common block.
      xx=xin
      yy=yin
      zz=zin
      tt=tin
c
      RETURN
      END
C     AUXILIARY TO QPYGIN0, BY KONRAD TYWONIUK.
      SUBROUTINE QPYJETPROD(X,Y)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER I,J
      REAL RPOS
      COMMON/OVERLAPW/WTAA(100,100),XM,YM
c
      rpos=pyr(0)
c
      sump=0.d0
      sumn=0.d0
      do i=1,100
         do j=1,100
            sumn=sumn+wtaa(j,i)
            if(rpos.gt.sump.and.rpos.lt.sumn) goto 14 ! finding jet prod point
            sump=sumn
         end do
      end do
 14   continue
      if(i.eq.100.and.j.eq.100) then
         PRINT*,'Failed to generate random jet position...'
c         STOP
      else
         x=dble(i-1)/99.d0*xm
         y=dble(j-1)/99.d0*ym
      end if
c     now decide which 1/4 part
      rpos=pyr(0)
      if (rpos .le. 0.25d0) then
         RETURN
      else if(rpos .gt. 0.25d0 .and. rpos .le. 0.5d0) then
         x=-1.d0*x
         RETURN
      else if(rpos .gt. 0.5d0 .and. rpos .le. 0.75d0) then
         x=-1.d0*x
         y=-1.d0*y
         RETURN
      else if(rpos .gt. 0.75d0 .and. rpos .le. 1.d0) then
         y=-1.d0*y
         RETURN
      endif
      END
c
      SUBROUTINE QPYGEO(X,Y,Z,T,BX,BY,BZ,QHL,OC) ! By Konrad Tywoniuk.
C     USER-DEFINED ROUTINE:
C     The values of qhatL and omegac have to be computed
C     by the user, using his preferred medium model, in
C     this routine, which takes as input the position
C     x,y,z,t of the parton to branch, the trajectory
C     defined by the three-vector bx,by,bz,
C     (all values in the center-of-mass frame of the
C     hard collision), and returns the value of qhatL
C     (in GeV**2) and omegac (in GeV).
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER MODEL
C     NOW THE COMMON CONTAINING THE VALUES OF THE TWO ANGLES AND THREE BOOST
C     PARAMETERS USED, IN QPYSHOW, TO CHANGE THROUGH PYROBO FROM THE
C     CENTER-OF-MASS OF THE COLLISION TO THE CENTER-OF-MASS OF THE HARD
C     SCATTERING. THEY ARE THE ENTRIES THREE TO SEVEN IN ROUTINE PYROBO.
      COMMON/QPLT/AA1,AA2,BBX,BBY,BBZ
      COMMON/MODPAR/ QHAT0,BETAX,BETAY,BETAZ,X0,Y0,Z0,TAU0,ETA0
      COMMON/FLOWEFFECT/ FLOWSTR,IFLOW
      COMMON/MODNO/ MODEL
c
      EXTERNAL QHATL,OMEGAC
c######################################################################
c
c     LIST OF MODELS (for "transverse" jets)
c
c######################################################################
      qhl=0.d0
      oc=0.d0
c... Without rotation (for consistency with prev. calculation)
      xr = x
      yr = y
      zr = z
      tr = t
      bxr = bx
      byr = by
      bzr = bz
      btr = 1.d0
c      print*,'pos1 in HARD frame: ',x,y,z,t,sqrt(x*x+y*y+z*z
c     &     -t*t)
c... Rotation of position and boost from the hard scattering frame
c... to the CM frame of the collision
c... (note that aa1 = -theta and aa2 = -phi, and also the
c...  boosts are multiplied by -1)
      call qpyrobo(x,y,z,t,-aa1,-aa2,-bbx,-bby,-bbz,xr,yr,zr,tr)
      call qpyrobo(bx,by,bz,1.d0,-aa1,-aa2,-bbx,-bby,-bbz,
     &     bxr,byr,bzr,btr)
c      print*,'Lorentz par: ',-aa1,-aa2,-bbx,-bby,-bbz
c      print*,'pos1 in COLL frame: ',xr,yr,zr,tr,sqrt(xr*xr+yr*yr+zr*zr
c     &     -tr*tr)
c      print*,' '
c     (1) for a parton at midrapidity inside a cylinder (approximate)
      if(model.eq.1) then
         xl = 10.d0             ! [fm]
         xlp = xl - dsqrt(xr*xr + yr*yr)
         if (xlp .lt. 0.d0) xlp = 0.d0
c     output values
         qhl = xlp*qhat0             ! [GeV**2]   
         oc = 0.5d0*qhl*xlp/0.1973d0 ! [GeV]
c-----NEW OPTIONS FOLLOW: "dynamical" models
      else if(model.ge.2 .and. model.le.6) then
         betax = bxr                  ! trajectory of particle
         betay = byr
         betaz = bzr
         x0 = xr                      ! particle position
         y0 = yr
         z0 = zr
         tau0 = dsqrt(tr**2 - zr**2)  ! tau=t when z=0
         eta0 = 0.5*dlog((tr + zr)/(tr - zr))
         if(tr.eq.zr.OR.(tr.eq.0.d0.and.zr.eq.0.d0)) eta0 = 0.d0
c     (2) "Bjorken model", cylinder with decreasing density 1/tau.
c     At the moment the cylinder is the same "for all centralities",
c     a more complex radius dependence can be parameterized as in 
c     HYDJET (e.g. 0711.0835; here e.g. R(b=0,phi) was 10 fm.)
c     The particle should move in the transverse (x-y) plane!
c-----bjorken+
         if(model.eq.2) then
            if(tau0.lt.10.d0) then
               qhl = dgauss2(qhatl,tau0,10.d0,1.d-5) ! [GeV**2]
               oc = dgauss2(omegac,tau0,10.d0,1.d-5)/0.197327d0 ! [GeV]
c               PRINT*,"Branching at: ",tau0,x0,y0,z0
c               PRINT*,tau0,x0,y0,z0,qhl,oc
            else
               qhl = 0.d0
               oc = 0.d0
            end if
c-----bjorken-
c     (3) 3D hydro from Hirano w/o flow effects
c     In this case, qhat is no parameter of the model but rather the
c     proportionality constant K, given by sigma = 2*K*epsilon**(3/4)
c-----hydro+
         else if(model.eq.3) then
            iflow = 0
            if(tau0.lt.17.1d0) call hhydro(qhl,oc) ! specially for hydro
c            PRINT*,tau0,x0,y0,z0,qhl,oc
c-----hydro-
c     (7) 3D hydro from Hirano with BMS flow effects on qhat
c     In this case, qhat is no parameter of the model but rather the
c     proportionality constant K, given by 
c     sigma = 2*K*epsilon**(3/4) * [(u dot p)/p0]
c-----hydroflow+
         else if(model.eq.4) then
            iflow = 1
            if(tau0.lt.17.1d0) call hhydro(qhl,oc) 
c            PRINT*,tau0,x0,y0,z0,qhl,oc
c-----hydroflow-
c     (4) 3D hydro from Hirano with ASW flow effects on qhat
c     In this case, qhat is no parameter of the model but rather the
c     proportionality constant K, given by 
c     sigma = 2*K*epsilon**(3/4) * (1 + |uT dot nT|**2)
c-----hydroflow2+
         else if(model.eq.5) then
            iflow = 2
            flowstr = 4.d0
            if(tau0.lt.17.1d0) call hhydro(qhl,oc) 
c            PRINT*,tau0,x0,y0,z0,qhl,oc
c-----hydroflow2-
c     (2) "PQM model", profile as overlap., qhat0 is the proportionality factor.
c     The particle should move in the transverse (x-y) plane!
c-----pqm+
         else if(model.eq.6) then
            qhl = dgauss2(qhatl,0.d0,15.d0,1.d-5) ! [GeV**2]
            oc = dgauss2(omegac,0.d0,15.d0,1.d-5)/0.197327d0 ! [GeV]
c            PRINT*,"Branching at: ",tau0,x0,y0,z0
c            PRINT*,'qhl,oc=',qhl,oc
         end if
c-----pqm-
      else
         print*, 'QPYGEO: NON EXISTING MEDIUM MODEL!!!'
         stop
      end if
      RETURN
      END
C
      FUNCTION QHATL(X)         ! ansatz
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER MODEL,TAMOD
      COMMON/MODPAR/ QHAT0,BETAX,BETAY,BETAZ,X0,Y0,Z0,TAU0,ETA0
      COMMON/MODNO/ MODEL
      COMMON/NUCL/ANUCL,RA,BPAR,TAMOD
c
      if (model .eq. 6) then
         taup=x
      else
         taup=x-tau0
      endif
      posx = x0+betax*taup
      posy = y0+betay*taup
c      
      if (model .eq. 6) then
         posa=dsqrt((posx-0.5d0*bpar)*(posx-0.5d0*bpar)+posy*posy)
         posb=dsqrt((posx+0.5d0*bpar)*(posx+0.5d0*bpar)+posy*posy)
         rho=profa(posa)*profa(posb)
      else
         posz = z0+betaz*taup
         eta = eta0+0.5d0*dlog((taup+posz)/(taup-posz))
         rho=rhobjork(x,posx,posy) 
      endif
c
      qhatl=qhat0*rho
c      
      RETURN
      END
c
      FUNCTION OMEGAC(X)        ! ansatz  
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER MODEL,TAMOD
      COMMON/MODPAR/ QHAT0,BETAX,BETAY,BETAZ,X0,Y0,Z0,TAU0,ETA0
      COMMON/MODNO/ MODEL
      COMMON/NUCL/ANUCL,RA,BPAR,TAMOD
c
      if (model .eq. 6) then
         taup=x
      else
         taup=x-tau0
      endif
      posx = x0+betax*taup
      posy = y0+betay*taup
c      
      if (model .eq. 6) then
         posa=dsqrt((posx-0.5d0*bpar)*(posx-0.5d0*bpar)+posy*posy)
         posb=dsqrt((posx+0.5d0*bpar)*(posx+0.5d0*bpar)+posy*posy)
         rho=profa(posa)*profa(posb)
      else
         posz = z0+betaz*taup
         eta = eta0+0.5d0*dlog((taup+posz)/(taup-posz))
         rho=rhobjork(x,posx,posy) 
      endif
c
      omegac=taup*qhat0*rho
c
      RETURN
      END
c
      FUNCTION RHOBJORK(T,X,Y)  ! Bjorken model
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (BJORKR=10.d0)
      rhobjork=0.d0
      if(t<0.1) then 
         rhobjork = 0.d0
      else 
         if(dsqrt(x**2+y**2).lt.bjorkr) rhobjork=0.1d0/t
      end if
      RETURN
      END
c
      SUBROUTINE HHYDRO(QHL,OC)
c
c     Since the hypersurfaces are given in intervals of 0.3fm
c     we simply sum up the contribution from all hypersurfaces
c     that contribute before freeze-out
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER itmax,it
c
      COMMON/MODPAR/ QHAT0,BETAX,BETAY,BETAZ,X0,Y0,Z0,TAU0,ETA0
      COMMON/FLOWEFFECT/ flowstr,iflow

      sctr = 197.32d0           ! hbar*c [Mev*fm] 

c      cq = 2.d0                 ! Const of prop to qhat (strong coupling regime)
      cq = qhat0                 ! Const of prop to qhat (strong coupling regime)
      dt = 0.3d0                ! Time step for hydro evolution [fm/c]
      itmax = 2000              ! Maximum number of time steps

c     from Hirano standard settings:
      tc = 170d0/sctr           ! Critical temperature in hydro simulation
      eqgp = 2184.81d0/sctr     ! Maximum energy density in the mixed phase
      ehad = 430.0d0/sctr       ! Minimum energy density in the mixed phase
      tfo= 100d0/sctr           ! Freezeout temperature in hydro simulation.
      
      qhl=0.d0
      oc=0.d0
c...Test+
      qhlm = 0.d0
      ocm = 0.d0
c...Test-
      
      if(abs(eta0).gt.2.d0) goto 100
      tau00=tau0
      if(tau00.lt.0.6d0) tau00=0.6d0 ! initial time in hydro model
c...Time evolution starts.(Time step is given by "dt")
      do it = 1,itmax
c...Assuming eikonal path
         tau = tau00+(it-1)*dt
         x   = x0+betax*dble(it-1)*dt
         y   = y0+betay*dble(it-1)*dt
         eta = eta0 
         if(tau.gt.17.d0) goto 100
c...Energy Density  [fm^-4]            
         ene = getene(tau,x,y,eta)/sctr*1d3
c         PRINT*,'Energy density at',tau,x,y,eta,' is ',ene
c...Particle Density [GeV^2/fm]
         rho=ene**(3.d0/4.d0)*0.03894d0
c...Temperature [fm^-1]
         temp = tc
         if((ene.gt.eqgp).or.(ene.lt.ehad)) then
            temp = gettemp(tau,x,y,eta)/sctr
         endif
c...Jet freeze-out defined by "t>tfo".
         if(temp.lt.tfo) goto 100
         
         if(iflow.eq.0) then
            qhl = qhl+2.d0*cq*rho*dt ! [GeV^2]
            oc = oc+2.d0*cq*(tau-tau0)*rho*dt/sctr*1d3 ! [GeV]
         else
c...Flow vector
c...Velocities
            vx = getvx(tau,x,y,eta)
            vy = getvy(tau,x,y,eta)
            yf = getyf(tau,x,y,eta)
c            vz = tanh(yf)

c$$$            if(vx.gt.0.4.OR.vy.gt.04) then 
c$$$               print*,'Large flow! ',vx,vy
c$$$               print*,ene,temp,tau,x,y,eta
c$$$               print*,' ---------------- '
c$$$            endif

            vperp = dsqrt(vx*vx + vy*vy)
            if(vperp.gt.1.d0) vperp = 0.d0
            gamp = 1.d0/dsqrt(1.d0 - vperp*vperp)
c...Flow 4-vector
            u0 = gamp*cosh(yf)
            ux = gamp*vx
            uy = gamp*vy
            uz = gamp*sinh(yf)

            sp = 0.d0
            if(iflow.eq.1) then
c...Baier,Mueller, Schiff flow modification of qhat
c...Scalar product: angle between flow and mom of particle
               sp = u0 - (ux*betax + uy*betay + uz*betaz)
            
            else if(iflow.eq.2) then
c...Armesto,Salgado,Wiedemann flow modification of qhat
c...Transverse part of flow
               sp = (ux*betay)**2 + (uy*betax)**2
               sp = 1.d0 + flowstr*sp
               
            endif

c...Modified qhat
            qhl = qhl+2.d0*cq*rho*sp*dt ! [GeV^2]
            oc = oc+2.d0*cq*(tau-tau0)*rho*sp*dt/sctr*1d3 ! [GeV]

c...Test (unmodified qhat calc)
            qhlm = qhlm+2.d0*cq*rho*dt ! [GeV^2]
            ocm = ocm+2.d0*cq*(tau-tau0)*rho*dt/sctr*1d3 ! [GeV]
         endif

      end do
 100  continue

c      print*,'Mod  qhatL = ',qhl
c      print*,'UMod qhatL = ',qhlm
      
      RETURN
      END

C
C     SAMPLE MAIN FOR Q-PYTHIA.1.0.3.
C     
C...Double precision and integer declarations.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      INTEGER PYK,PYCHGE,PYCOMP
      INTEGER IDUM
      INTEGER MODEL,TAMOD
      integer today(3), now(3)
      real aaa(1)
      CHARACTER*80 CENT         ! centrality for 3D hydro
      CHARACTER*40 FILEINPUT, FILENAME

C...EXTERNAL statement links PYDATA on most machines.
      EXTERNAL PYDATA
      EXTERNAL readdata         ! 3D hydro data routine

C...Commonblocks.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/MODPAR/QHAT0,BETAX,BETAY,BETAZ,X0,Y0,Z0,TAU0
      COMMON/MODNO/MODEL
      COMMON/FILE/CENT          ! 3D hydro centrality common
      COMMON/NUCL/ANUCL,RA,BPAR,TAMOD
      common/flcoh/iflcoh ! 1.0.3
C
      NARGS = IARGC()
      IF(NARGS.NE.2) THEN
         PRINT*,'NEED TO SPECIFY INPUT AND OUTPUT FILES!!!'
         STOP
      END IF
C     LIST OF ARGUMENTS TO BE GIVEN:
C     1: INPUT FILE
C     2: OUTPUT FILE
      CALL GETARG(1,FILEINPUT)
      CALL GETARG(2,FILENAME)
      OPEN(60,FILE=FILEINPUT,STATUS='unknown')
      READ(60,*) QHAT0
      READ(60,*) MODEL
      READ(60,*) ANUCL
      READ(60,*) BPAR
      READ(60,*) CENT
      READ(60,*) TAMOD
      READ(60,*) IFLCOH
      READ(60,*) NEV
      PRINT*, 'QHAT0, MODEL, ANUCL, BPAR, CENT, TAMOD, IFLCOH, NEV='
      PRINT*, QHAT0, MODEL, ANUCL, BPAR, CENT, TAMOD, IFLCOH, NEV
      CLOSE(60)
      OPEN(70,FILE=FILENAME,STATUS='unknown')
c

c     qhat0=5.0d0               ! qhat (for Bjorken mod qhat0) in [GeV**2/fm]
c      qhat0=14000.0d0           ! GeV**2 fm**3, qhat for PQM, approximate for <qhat>=1 
c      model=6                   ! type of medium (1: cylinder,2: Bjorken,3,4,5: 3D hydro, 6: PQM)
c
c      anucl=208.d0              ! atomic number
      ra=1.07d0*anucl**0.3333d0 ! nuclear radius
c      bpar=3.1d0                 ! impact parameter of AA collision 
c      cent='0-10'                ! centrality class (for hydro)
      if(model.ge.3 .and. model.le.5) call readdata ! Read and store hydro data table
c      tamod=2                   ! 1: particle prod from center, 2: nuclear overlap 
      call qpyini               ! initialize the jet prod array
c
C...Number of events.
c      NEV=1
c
c   switches (default refers to PYTHIA-6.4.18 defaults).
c
c   MSTJ(47) and MSTJ(50) are fixed this way because we are running a
c   single parton which does not come from any hard scattering.
c
c   MSTJ(41) is fixed this way because we are only interested in QCD radiation.
c
c   MSTJ(41) must NOT be 11 or 12, as then FSR may go through PYPTFS
c   (kt-ordered cascade) in which medium effects have not been introduced.
c
c      MSTJ(41)=1 ! QCD radiation only (default=2)
c      MSTJ(42)=2 ! 2: angular ordering (default), 1: no angular ordering
c      MSTJ(44)=2 ! options to run alphas (default=2)
c      MSTJ(47)=0 ! no correction back to hard scattering element (default=3)
c      MSTJ(50)=0 ! no coherence in first branching (default=3)
c      PARJ(82)=1.D0 ! GeV, cut off for parton showers (default=1 GeV)
c
c     LHA for jet definition
      mstj(22)=2
      parj(71)=3.d0
c     QCD jets, large pt demanded
      MSEL=1
      CKIN(3)=10.d0
c     
c     DW tune, hep-ph/0610012
c      MSTP(51)=7 ! CTEQ5L
c      MSTP(81)=1 ! multiple interactions on
c      MSTP(82)=4 ! varying impact parameter, double gaussian for multiple int.
c      PARP(82)=1.9d0 ! regularization of transverse momentum for multiple int.
c      PARP(83)=0.5d0 ! parameters of the double gaussian overlap
c      PARP(84)=0.4d0 ! parameters of the double gaussian overlap
c      PARP(85)=1.0d0 ! probability of additional two gluons in mutiple int.
c      PARP(86)=1.d0  ! probability of additional two gluons in mutiple int.
c      PARP(89)=1800.d0 ! reference energy scale
c      PARP(90)=0.25d0 ! power of energy rescaling of parp(82)
c      PARP(62)=1.25d0 ! cut-off for space-like parton showers
c      PARP(64)=0.2d0 ! scale for alphas evaluation in space-like parton showers
c      PARP(67)=2.5d0 ! scale for maximum virtuality on space-like parton showers
c      MSTP(91)=1 ! Gaussian primordial kt distribution in hadron
c      PARP(91)=2.1d0 ! width of Gaussian primordial kt
c      PARP(93)=15.0d0 ! upper cut-off for primordial kt in the hadron
c
c     to ensure different generations at each time.
      call idate(today)
      call itime(now)
      CALL RLUXGO(4,today(1),0,0)
      call ranlux(aaa,1)
      dx1=dble(aaa(1))
      CALL RLUXGO(4,today(2),0,0)
      call ranlux(aaa,1)
      dx2=dble(aaa(1))
      CALL RLUXGO(4,today(3),0,0)
      call ranlux(aaa,1)
      dx3=dble(aaa(1))
      CALL RLUXGO(4,now(1),0,0)
      call ranlux(aaa,1)
      dx4=dble(aaa(1))
      CALL RLUXGO(4,now(2),0,0)
      call ranlux(aaa,1)
      dx5=dble(aaa(1))
      CALL RLUXGO(4,now(3),0,0)
      call ranlux(aaa,1)
      dx6=dble(aaa(1))
      isss=int(2147483647*(dx1+dx2+dx3+dx4+dx5+dx6)/6.d0)
      CALL RLUXGO(3,isss,0,0)
      print*, 'SEED FOR RANLUX=', ISSS

C************INITIALIZATION *************
c      call PYINIT('CMS','p','p',2.76d+3) ! LHC collisions

C...Event loop. List first few events.
      write(70,*) nev
      DO 1 IEV=1,NEV
c
         call qpygin0 ! generate initial nucleon-nucleon position.
c
         IF(MOD(IEV,100).eq.0) PRINT*,"Loop # ",IEV
c
         call PYEVNT ! Generate one PYTHIA event.
c    LHA for jet definition
         CALL PYEDIT(1)
c
         IF(IEV.LE.5) CALL PYLIST(1)
c
         WRITE(70,*) IEV, N
         DO 2 Iqq=1, N, 1
c          if(k(iqq,2).ge.111) then      !writes only mesons and baryons
            WRITE(70,*) K(iqq,2), P(Iqq,1), P(Iqq,2), P(Iqq,3),
     >                  P(Iqq,4), P(Iqq,5)
c          endif  
 2         CONTINUE
c     
 1    CONTINUE
c
      CLOSE(70)
c
C...Final statistics and histograms.
c      CALL PYSTAT(1)
c
      STOP
      END

C     Initialization of a nucleus-nucleus collision geometry
      SUBROUTINE QPYINI
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)   
      INTEGER I,J,TAMOD
      EXTERNAL RHOA
      COMMON/NUCL/ANUCL,RA,BPAR,TAMOD
      COMMON/RHO/BB
      COMMON/OVERLAPW/WTAA(100,100),XM,YM
      COMMON/PROFILE/BARR(100),TARR(100)

c     for z-integration of WS densities
      z2=4.d0*ra
      z1=-1.d0*z2

c     creating a regular grid for nuclear overlap
c     NOTE: only a quarter of the overlap is covered
      xmin=0.d0
      xmax=ra-bpar/2.d0
      xm=xmax
      if(xmax.lt.xmin) then
         PRINT*,'Too large impact parameter: b/2>RA!'
c         STOP
      end if
      ymin=0.d0
      ymax=dsqrt(ra**2-bpar**2/4.d0) ! only for b/2 < ra
      ym=ymax
      xdel=(xmax-xmin)/99.d0
      ydel=(ymax-ymin)/99.d0
      
      wtaasum=0.d0
      do i=1,100
         x=xmin+(i-1)*xdel
         do j=1,100
            y=ymin+(j-1)*ydel
            
            r=dsqrt(x**2+y**2)
            if(r.eq.0.d0) then 
               theta=0.d0
            else theta=acos(x/r)
            endif
            
c     distances from centra of the two nuclei
            bb=dsqrt(abs(r*r+bpar*bpar/4.d0+r*bpar*dcos(theta)))
            ta1=dgauss2(rhoa,z1,z2,1.d-4)
            bb=dsqrt(abs(r*r+bpar*bpar/4.d0-r*bpar*dcos(theta)))
            ta2=dgauss2(rhoa,z1,z2,1.d-4)

            wtaa(j,i)=ta1*ta2
            wtaasum=wtaasum+wtaa(j,i)
         end do
      end do

c     normalize the weights
      do i=1,100
         do j=1,100
            wtaa(j,i)=wtaa(j,i)/wtaasum
         end do
      end do

c     profile up to 2*ra
      deltaprof=2.d0*ra/99.d0
      checknprof=0.d0
      do i=1, 100, 1
         bb=dble(i-1)*deltaprof
         barr(i)=bb
         ttarr=dgauss2(rhoa,z1,z2,1.d-4)
         tarr(i)=ttarr
         if (i .gt. 1) checknprof=checknprof+
     >                deltaprof*2.d0*dacos(-1.d0)*barr(i)*tarr(i)
c         print*,bb,tarr(i)
      enddo
      print*, 'normalization of profile=', checknprof
c      
      RETURN
      END
c
      FUNCTION RHOA(Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INTEGER TAMOD
      COMMON/NUCL/ANUCL,RA,BPAR,TAMOD
      COMMON/RHO/BB
      PARAMETER(DF=0.545d0, PI=3.141593d0)
c
      r=dsqrt(bb**2+z**2)
      rho0=3.d0/(4.d0*pi*ra**3)/(1.d0+(pi*df/ra)**2)
      rhoa=rho0/(1.d0+dexp((r-ra)/df))
c
      RETURN
      END
c
      FUNCTION PROFA(XB)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)   
      COMMON/PROFILE/BARR(100),TARR(100)
      PROFA=DDIVDIF(TARR,BARR,100,XB,1)
      RETURN
      END

      SUBROUTINE RANLUX(RVEC,LENV)
C         Subtract-and-borrow random number generator proposed by
C         Marsaglia and Zaman, implemented by F. James with the name
C         RCARRY in 1991, and later improved by Martin Luescher
C         in 1993 to produce "Luxury Pseudorandom Numbers".
C     Fortran 77 coded by F. James, 1993
C
C   LUXURY LEVELS.
C   ------ ------      The available luxury levels are:
C
C  level 0  (p=24): equivalent to the original RCARRY of Marsaglia
C           and Zaman, very long period, but fails many tests.
C  level 1  (p=48): considerable improvement in quality over level 0,
C           now passes the gap test, but still fails spectral test.
C  level 2  (p=97): passes all known tests, but theoretically still
C           defective.
C  level 3  (p=223): DEFAULT VALUE.  Any theoretically possible
C           correlations have very small chance of being observed.
C  level 4  (p=389): highest possible luxury, all 24 bits chaotic.
C
C!!! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C!!!  Calling sequences for RANLUX:                                  ++
C!!!      CALL RANLUX (RVEC, LEN)   returns a vector RVEC of LEN     ++
C!!!                   32-bit random floating point numbers between  ++
C!!!                   zero (not included) and one (also not incl.). ++
C!!!      CALL RLUXGO(LUX,INT,K1,K2) initializes the generator from  ++
C!!!               one 32-bit integer INT and sets Luxury Level LUX  ++
C!!!               which is integer between zero and MAXLEV, or if   ++
C!!!               LUX .GT. 24, it sets p=LUX directly.  K1 and K2   ++
C!!!               should be set to zero unless restarting at a break++ 
C!!!               point given by output of RLUXAT (see RLUXAT).     ++
C!!!      CALL RLUXAT(LUX,INT,K1,K2) gets the values of four integers++
C!!!               which can be used to restart the RANLUX generator ++
C!!!               at the current point by calling RLUXGO.  K1 and K2++
C!!!               specify how many numbers were generated since the ++
C!!!               initialization with LUX and INT.  The restarting  ++
C!!!               skips over  K1+K2*E9   numbers, so it can be long.++
C!!!   A more efficient but less convenient way of restarting is by: ++
C!!!      CALL RLUXIN(ISVEC)    restarts the generator from vector   ++
C!!!                   ISVEC of 25 32-bit integers (see RLUXUT)      ++
C!!!      CALL RLUXUT(ISVEC)    outputs the current values of the 25 ++
C!!!                 32-bit integer seeds, to be used for restarting ++
C!!!      ISVEC must be dimensioned 25 in the calling program        ++
C!!! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      DIMENSION RVEC(LENV)
      DIMENSION SEEDS(24), ISEEDS(24), ISDEXT(25)
      PARAMETER (MAXLEV=4, LXDFLT=3)
      DIMENSION NDSKIP(0:MAXLEV)
      DIMENSION NEXT(24)
      PARAMETER (TWOP12=4096., IGIGA=1000000000,JSDFLT=314159265)
      PARAMETER (ITWO24=2**24, ICONS=2147483563)
      SAVE NOTYET, I24, J24, CARRY, SEEDS, TWOM24, TWOM12, LUXLEV
      SAVE NSKIP, NDSKIP, IN24, NEXT, KOUNT, MKOUNT, INSEED
      INTEGER LUXLEV
      LOGICAL NOTYET
      DATA NOTYET, LUXLEV, IN24, KOUNT, MKOUNT /.TRUE., LXDFLT, 0,0,0/
      DATA I24,J24,CARRY/24,10,0./
C                               default
C  Luxury Level   0     1     2   *3*    4
      DATA NDSKIP/0,   24,   73,  199,  365 /
Corresponds to p=24    48    97   223   389
C     time factor 1     2     3     6    10   on slow workstation
C                 1    1.5    2     3     5   on fast mainframe
C
C  NOTYET is .TRUE. if no initialization has been performed yet.
C              Default Initialization by Multiplicative Congruential
      IF (NOTYET) THEN
         NOTYET = .FALSE.
         JSEED = JSDFLT  
         INSEED = JSEED
         WRITE(6,'(A,I12)') ' RANLUX DEFAULT INITIALIZATION: ',JSEED
         LUXLEV = LXDFLT
         NSKIP = NDSKIP(LUXLEV)
         LP = NSKIP + 24
         IN24 = 0
         KOUNT = 0
         MKOUNT = 0
         WRITE(6,'(A,I2,A,I4)')  ' RANLUX DEFAULT LUXURY LEVEL =  ',
     +        LUXLEV,'      p =',LP
            TWOM24 = 1.
         DO 25 I= 1, 24
            TWOM24 = TWOM24 * 0.5
         K = JSEED/53668
         JSEED = 40014*(JSEED-K*53668) -K*12211
         IF (JSEED .LT. 0)  JSEED = JSEED+ICONS
         ISEEDS(I) = MOD(JSEED,ITWO24)
   25    CONTINUE
         TWOM12 = TWOM24 * 4096.
         DO 50 I= 1,24
         SEEDS(I) = REAL(ISEEDS(I))*TWOM24
         NEXT(I) = I-1
   50    CONTINUE
         NEXT(1) = 24
         I24 = 24
         J24 = 10
         CARRY = 0.
         IF (SEEDS(24) .EQ. 0.) CARRY = TWOM24
      ENDIF
C
C          The Generator proper: "Subtract-with-borrow",
C          as proposed by Marsaglia and Zaman,
C          Florida State University, March, 1989
C
      DO 100 IVEC= 1, LENV
      UNI = SEEDS(J24) - SEEDS(I24) - CARRY 
      IF (UNI .LT. 0.)  THEN
         UNI = UNI + 1.0
         CARRY = TWOM24
      ELSE
         CARRY = 0.
      ENDIF
      SEEDS(I24) = UNI
      I24 = NEXT(I24)
      J24 = NEXT(J24)
      RVEC(IVEC) = UNI
C  small numbers (with less than 12 "significant" bits) are "padded".
      IF (UNI .LT. TWOM12)  THEN
         RVEC(IVEC) = RVEC(IVEC) + TWOM24*SEEDS(J24)
C        and zero is forbidden in case someone takes a logarithm
         IF (RVEC(IVEC) .EQ. 0.)  RVEC(IVEC) = TWOM24*TWOM24
      ENDIF
C        Skipping to luxury.  As proposed by Martin Luscher.
      IN24 = IN24 + 1
      IF (IN24 .EQ. 24)  THEN
         IN24 = 0
         KOUNT = KOUNT + NSKIP
         DO 90 ISK= 1, NSKIP
         UNI = SEEDS(J24) - SEEDS(I24) - CARRY
         IF (UNI .LT. 0.)  THEN
            UNI = UNI + 1.0
            CARRY = TWOM24
         ELSE
            CARRY = 0.
         ENDIF
         SEEDS(I24) = UNI
         I24 = NEXT(I24)
         J24 = NEXT(J24)
   90    CONTINUE
      ENDIF
  100 CONTINUE
      KOUNT = KOUNT + LENV
      IF (KOUNT .GE. IGIGA)  THEN
         MKOUNT = MKOUNT + 1
         KOUNT = KOUNT - IGIGA
      ENDIF
      RETURN
C
C           Entry to input and float integer seeds from previous run
      ENTRY RLUXIN(ISDEXT)
         NOTYET = .FALSE.
         TWOM24 = 1.
         DO 195 I= 1, 24
         NEXT(I) = I-1
  195    TWOM24 = TWOM24 * 0.5
         NEXT(1) = 24
         TWOM12 = TWOM24 * 4096.
      WRITE(6,'(A)') ' FULL INITIALIZATION OF RANLUX WITH 25 INTEGERS:'
      WRITE(6,'(5X,5I12)') ISDEXT
      DO 200 I= 1, 24
      SEEDS(I) = REAL(ISDEXT(I))*TWOM24
  200 CONTINUE
      CARRY = 0.
      IF (ISDEXT(25) .LT. 0)  CARRY = TWOM24
      ISD = IABS(ISDEXT(25))
      I24 = MOD(ISD,100)
      ISD = ISD/100
      J24 = MOD(ISD,100)
      ISD = ISD/100
      IN24 = MOD(ISD,100)
      ISD = ISD/100
      LUXLEV = ISD
        IF (LUXLEV .LE. MAXLEV) THEN
          NSKIP = NDSKIP(LUXLEV)
          WRITE (6,'(A,I2)') ' RANLUX LUXURY LEVEL SET BY RLUXIN TO: ',
     +                         LUXLEV
        ELSE  IF (LUXLEV .GE. 24) THEN
          NSKIP = LUXLEV - 24
          WRITE (6,'(A,I5)') ' RANLUX P-VALUE SET BY RLUXIN TO:',LUXLEV
        ELSE
          NSKIP = NDSKIP(MAXLEV)
          WRITE (6,'(A,I5)') ' RANLUX ILLEGAL LUXURY RLUXIN: ',LUXLEV
          LUXLEV = MAXLEV
        ENDIF
      INSEED = -1
      RETURN
C
C                    Entry to ouput seeds as integers
      ENTRY RLUXUT(ISDEXT)
      DO 300 I= 1, 24
         ISDEXT(I) = INT(SEEDS(I)*TWOP12*TWOP12)
  300 CONTINUE
      ISDEXT(25) = I24 + 100*J24 + 10000*IN24 + 1000000*LUXLEV
      IF (CARRY .GT. 0.)  ISDEXT(25) = -ISDEXT(25)
      RETURN
C
C                    Entry to output the "convenient" restart point
      ENTRY RLUXAT(LOUT,INOUT,K1,K2)
      LOUT = LUXLEV
      INOUT = INSEED
      K1 = KOUNT
      K2 = MKOUNT
      RETURN
C
C                    Entry to initialize from one or three integers
      ENTRY RLUXGO(LUX,INS,K1,K2)
         IF (LUX .LT. 0) THEN
            LUXLEV = LXDFLT
         ELSE IF (LUX .LE. MAXLEV) THEN
            LUXLEV = LUX
         ELSE IF (LUX .LT. 24 .OR. LUX .GT. 2000) THEN
            LUXLEV = MAXLEV
            WRITE (6,'(A,I7)') ' RANLUX ILLEGAL LUXURY RLUXGO: ',LUX
         ELSE
            LUXLEV = LUX
            DO 310 ILX= 0, MAXLEV
              IF (LUX .EQ. NDSKIP(ILX)+24)  LUXLEV = ILX
  310       CONTINUE
         ENDIF
      IF (LUXLEV .LE. MAXLEV)  THEN
         NSKIP = NDSKIP(LUXLEV)
         WRITE(6,'(A,I2,A,I4)') ' RANLUX LUXURY LEVEL SET BY RLUXGO :',
     +        LUXLEV,'     P=', NSKIP+24
      ELSE
          NSKIP = LUXLEV - 24
          WRITE (6,'(A,I5)') ' RANLUX P-VALUE SET BY RLUXGO TO:',LUXLEV
      ENDIF
      IN24 = 0
      IF (INS .LT. 0)  WRITE (6,'(A)')   
     +   ' Illegal initialization by RLUXGO, negative input seed'
      IF (INS .GT. 0)  THEN
        JSEED = INS
        WRITE(6,'(A,3I12)') ' RANLUX INITIALIZED BY RLUXGO FROM SEEDS',
     +      JSEED, K1,K2
      ELSE
        JSEED = JSDFLT
        WRITE(6,'(A)')' RANLUX INITIALIZED BY RLUXGO FROM DEFAULT SEED'
      ENDIF
      INSEED = JSEED
      NOTYET = .FALSE.
      TWOM24 = 1.
         DO 325 I= 1, 24
           TWOM24 = TWOM24 * 0.5
         K = JSEED/53668
         JSEED = 40014*(JSEED-K*53668) -K*12211
         IF (JSEED .LT. 0)  JSEED = JSEED+ICONS
         ISEEDS(I) = MOD(JSEED,ITWO24)
  325    CONTINUE
      TWOM12 = TWOM24 * 4096.
         DO 350 I= 1,24
         SEEDS(I) = REAL(ISEEDS(I))*TWOM24
         NEXT(I) = I-1
  350    CONTINUE
      NEXT(1) = 24
      I24 = 24
      J24 = 10
      CARRY = 0.
      IF (SEEDS(24) .EQ. 0.) CARRY = TWOM24
C        If restarting at a break point, skip K1 + IGIGA*K2
C        Note that this is the number of numbers delivered to
C        the user PLUS the number skipped (if luxury .GT. 0).
      KOUNT = K1
      MKOUNT = K2
      IF (K1+K2 .NE. 0)  THEN
        DO 500 IOUTER= 1, K2+1
          INNER = IGIGA
          IF (IOUTER .EQ. K2+1)  INNER = K1
          DO 450 ISK= 1, INNER
            UNI = SEEDS(J24) - SEEDS(I24) - CARRY 
            IF (UNI .LT. 0.)  THEN
               UNI = UNI + 1.0
               CARRY = TWOM24
            ELSE
               CARRY = 0.
            ENDIF
            SEEDS(I24) = UNI
            I24 = NEXT(I24)
            J24 = NEXT(J24)
  450     CONTINUE
  500   CONTINUE
C         Get the right value of IN24 by direct calculation
        IN24 = MOD(KOUNT, NSKIP+24)
        IF (MKOUNT .GT. 0)  THEN
           IZIP = MOD(IGIGA, NSKIP+24)
           IZIP2 = MKOUNT*IZIP + IN24
           IN24 = MOD(IZIP2, NSKIP+24)
        ENDIF
C       Now IN24 had better be between zero and 23 inclusive
        IF (IN24 .GT. 23) THEN
           WRITE (6,'(A/A,3I11,A,I5)')  
     +    '  Error in RESTARTING with RLUXGO:','  The values', INS,
     +     K1, K2, ' cannot occur at luxury level', LUXLEV
           IN24 = 0
        ENDIF
      ENDIF
      RETURN
      END
 
