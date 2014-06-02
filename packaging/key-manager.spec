Name:       key-manager
Summary:    Central Key Manager and utilities
Version:    0.0.1
Release:    1
Group:      System/Security
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires: cmake
BuildRequires: zip
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(openssl)
BuildRequires: libattr-devel
BuildRequires: pkgconfig(libsmack)
BuildRequires: pkgconfig(libsystemd-daemon)
%{?systemd_requires}

%description
Central Key Manager and utilities

%package -n libkey-manager-client
Summary:    Central Key Manager (client)
Group:      Development/Libraries
Requires:   key-manager = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libkey-manager-client
Central Key Manager package (client)

%package -n libkey-manager-client-devel
Summary:    Central Key Manager (client-devel)
Group:      Development/Libraries
Requires:   libkey-manager-client = %{version}-%{release}

%description -n libkey-manager-client-devel
Central Key Manager package (client-devel)

%package -n key-manager-devel
Summary:    Central Key Manager (Development)
Group:      Development/Libraries
Requires:   key-manager = %{version}-%{release}

%description -n key-manager-devel
Central Key Manager (Development)


%prep
%setup -q


%build
%if 0%{?sec_build_binary_debug_enable}
    export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
    export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
    export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif


export LDFLAGS+="-Wl,--rpath=%{_libdir} "

%cmake . -DVERSION=%{version} \
        -DCMAKE_BUILD_TYPE=%{?build_type:%build_type}%{!?build_type:RELEASE} \
        -DCMAKE_VERBOSE_MAKEFILE=ON 
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}
cp LICENSE %{buildroot}/usr/share/license/libkey-manager-client
mkdir -p %{buildroot}/etc/security/

%make_install
mkdir -p %{buildroot}/usr/lib/systemd/system/multi-user.target.wants
mkdir -p %{buildroot}/usr/lib/systemd/system/sockets.target.wants
ln -s ../central-key-manager.service %{buildroot}/usr/lib/systemd/system/multi-user.target.wants/central-key-manager.service
ln -s ../central-key-manager-echo.socket %{buildroot}/usr/lib/systemd/system/sockets.target.wants/central-key-manager-echo.socket
ln -s ../central-key-manager-api-control.socket %{buildroot}/usr/lib/systemd/system/sockets.target.wants/central-key-manager-api-control.socket
ln -s ../central-key-manager-api-storage.socket %{buildroot}/usr/lib/systemd/system/sockets.target.wants/central-key-manager-api-storage.socket


%clean
rm -rf %{buildroot}

%post
systemctl daemon-reload
if [ $1 = 1 ]; then
    # installation
    systemctl start key-manager.service
fi

if [ $1 = 2 ]; then
    # update
    systemctl restart key-manager.service
fi


%preun
if [ $1 = 0 ]; then
    # unistall
    systemctl stop key-manager.service
fi

%postun
if [ $1 = 0 ]; then
    # unistall
    systemctl daemon-reload
fi

%post -n libkey-manager-client -p /sbin/ldconfig

%postun -n libkey-manager-client -p /sbin/ldconfig

%files -n key-manager
%manifest %{_datadir}/key-manager.manifest
%attr(755,root,root) /usr/bin/key-manager
%{_libdir}/libkey-manager-commons.so.*
%{_libdir}/libkey-manager-key-provider.so.*
%{_libdir}/libkey-manager-key-provider.so
%attr(-,root,root) /usr/lib/systemd/system/multi-user.target.wants/central-key-manager.service
%attr(-,root,root) /usr/lib/systemd/system/central-key-manager.service
%attr(-,root,root) /usr/lib/systemd/system/central-key-manager.target
%attr(-,root,root) /usr/lib/systemd/system/sockets.target.wants/central-key-manager-echo.socket
%attr(-,root,root) /usr/lib/systemd/system/central-key-manager-echo.socket
%attr(-,root,root) /usr/lib/systemd/system/sockets.target.wants/central-key-manager-api-control.socket
%attr(-,root,root) /usr/lib/systemd/system/central-key-manager-api-control.socket
%attr(-,root,root) /usr/lib/systemd/system/sockets.target.wants/central-key-manager-api-storage.socket
%attr(-,root,root) /usr/lib/systemd/system/central-key-manager-api-storage.socket
%{_datadir}/license/%{name}

%files -n libkey-manager-client
%manifest %{_datadir}/libkey-manager-client.manifest
%defattr(-,root,root,-)
%{_libdir}/libkey-manager-client.so.*
%{_datadir}/license/libkey-manager-client

%files -n libkey-manager-client-devel
%defattr(-,root,root,-)
%{_libdir}/libkey-manager-client.so
%{_libdir}/libkey-manager-commons.so
%{_includedir}/ckm/ckm/key-manager.h
%{_includedir}/ckm/ckm/ckm-error.h
%{_includedir}/ckm/ckm/ckm-type.h
%{_libdir}/pkgconfig/*.pc