# 🧠 Malloc Implementation Project

Bu proje, C programlama dilinde dinamik bellek ayırma mekanizmasını (malloc) sıfırdan implement etmektedir.

## 📋 Proje Özellikleri

### ✅ Temel Fonksiyonlar
- `malloc(size_t size)` - Bellek ayırma
- `free(void *ptr)` - Bellek serbest bırakma
- `realloc(void *ptr, size_t size)` - Bellek yeniden boyutlandırma

### 🏗️ Mimari Tasarım
- **TINY Zone**: 1-128 byte allocationlar için optimize edilmiş
- **SMALL Zone**: 129-1024 byte allocationlar için optimize edilmiş
- **LARGE Zone**: 1024+ byte allocationlar için doğrudan mmap

### 🚀 Optimizasyonlar
- Pre-allocation stratejisi ile sistem çağrılarını minimize etme
- Block coalescing ile fragmentation önleme
- Efficient alignment (16-byte) uygulaması
- Zone-based memory management

## 🛠️ Kullanım

### Derleme
```bash
make                   # Projeyi derler
make clean             # Object dosyalarını temizler
make fclean            # Tüm derleme çıktılarını temizler
make re                # Temizler ve yeniden derler
```

### Test Etme
```bash
make test              # Testleri çalıştırabileceğin bir program başlatır.
```

### Kütüphane Kullanımı
```bash
# Program derleme
gcc program.c -L. -lft_malloc -o program

# Çalıştırma
LD_LIBRARY_PATH=. ./program
```

## Zone Sizes
- **TINY**: 16KB zonlar (~100 allocation)
- **SMALL**: 64KB zonlar (~100 allocation)
- **LARGE**: Dynamic sizing per allocation

### Alignment
- Tüm allocationlar 16-byte hizalı
- Performans ve compatibility için kritik
